#include "UR_control/ServerNode.h"
#include "ros/console.h"
#include <optional>
#include <functional>

//std::string ServerNode::program_name = "";

std::unique_ptr<RobotConnection> ServerNode::robotConnection = nullptr;

ServerNode::ServerNode(int argc, char **argv)
{
	ros::init(argc, argv, "robot_command_server");
	node = std::make_unique<ros::NodeHandle>();
    if( ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug) )
    ros::console::notifyLoggerLevelsChanged();

	ros::NodeHandle nh_priv("~");
	bool roboter_ipSet = nh_priv.getParam("roboter_ip", roboter_ip);
	bool roboter_portSet = nh_priv.getParam("roboter_port", roboter_port);
	//bool program_nameSet = nh_priv.getParam("program_name", program_name);

	if (!roboter_ipSet || !roboter_portSet) // || !program_nameSet)
	{
		ROS_ERROR("Please set roboter_ipSet, roboter_portSet and program_nameSet in the .launch file");
		ROS_INFO("%s", roboter_ip.c_str());
		ros::shutdown();
	}
}

void ServerNode::start()
{
	ros::ServiceServer server = node->advertiseService("robotCommand", robotCommand);
	ROS_INFO("robotCommand service activated.");

	robotConnection = std::make_unique<RobotConnection>(roboter_ip, roboter_port);
	ROS_INFO("Connection established.");
	ROS_INFO("Robot response:\n%s", robotConnection->receive().c_str());

	ros::spin();
}

bool ServerNode::robotCommand(UR_control::robotCommandRequest &request, UR_control::robotCommandResponse &response)
{
	std::string command;
	std::function<std::string(const std::string& command)> handleCommandRequest;
	handleCommandRequest = &sendCommandToRobot;

	switch (request.command){
        case request.LOAD:
            command = std::string("load ").append(request.program); //kann leer sein ->Standardwert wird momentan nicht gesetzt in diesem Fall
            break;
		case request.PLAY:
			command = std::string("play");
			handleCommandRequest = &requestCommandPLAY;
			break;
        case request.STOP:
            command = std::string("stop");
            break;
        case request.PAUSE:
            command = std::string("pause");
            break;
        case request.RUNNING:
            command = std::string("running");
            break;
        default:
			response.response = "Incorrect command";
			ROS_ERROR("%s", response.response.c_str());
			return true;
	}

	ROS_INFO("Received command:\n%s", command.c_str());
	response.response = handleCommandRequest(command);

	return true;
}

std::string ServerNode::requestCommandPLAY(const std::string& command)
{
	std::string isRunningString = sendCommandToRobot("running");
	std::string response;

	if( isRunningString.find("true") != std::string::npos ) { //weiteres Mal
		//XMLRPC Server benachrichtigen


		response = "Robot is already running. Notified XMLRPC Server.";
	} else if ( isRunningString.find("false") != std::string::npos ) { // erstes Mal
		response = sendCommandToRobot("play");
	} else { //Fehler
		response = "Robot message not interpretable: " + isRunningString;
		ROS_ERROR("%s", response.c_str());
	}

	return response;
}

std::string ServerNode::sendCommandToRobot(const std::string& command)
{
	std::string alteredCommand(command);
	alteredCommand.append("\n"); //Anforderung vom UR Dashboard Server
	
	robotConnection->send(alteredCommand);

	std::string response = robotConnection->receive();
	ROS_INFO("Robot response:\n%s", response.c_str());

	return response;
}


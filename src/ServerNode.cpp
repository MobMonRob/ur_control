#include "UR_control/ServerNode.h"
#include "ros/console.h"
#include <functional>

std::string ServerNode::program_name = "";

std::unique_ptr<RobotConnection> ServerNode::robotConnection = nullptr;
std::unique_ptr<XmlRpc::XmlRpcClient> ServerNode::xmlRpcClient = nullptr;

ServerNode::ServerNode(int argc, char **argv)
{
	ros::init(argc, argv, "robot_command_server");
	node = std::make_unique<ros::NodeHandle>();
    if( ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug) )
    ros::console::notifyLoggerLevelsChanged();

	ros::NodeHandle nh_priv("~");
	bool roboter_ipSet = nh_priv.getParam("roboter_ip", roboter_ip);
	bool roboter_portSet = nh_priv.getParam("roboter_port", roboter_port);
	nh_priv.getParam("program_name", program_name);
	bool xmlrpc_ipSet = nh_priv.getParam("xmlrpc_ip", xmlrpc_ip);
	bool xmlrpc_portSet = nh_priv.getParam("xmlrpc_port", xmlrpc_port);

	if (!roboter_ipSet || !roboter_portSet || !xmlrpc_ipSet || !xmlrpc_portSet)
	{
		ROS_ERROR("Error: Please configure .launch file properly.");
		ROS_INFO("%s", roboter_ip.c_str());
		ros::shutdown();
	}
}

void ServerNode::start()
{
	std::unique_ptr<SocketConnection> socketConnection = std::make_unique<SocketConnection>(roboter_ip, roboter_port);
	ROS_INFO("Connection established.");
	ROS_INFO("Robot response:\n%s", socketConnection->receive().c_str());

	robotConnection = std::make_unique<RobotConnection>(socketConnection);

	xmlRpcClient = std::make_unique<XmlRpc::XmlRpcClient>(xmlrpc_ip.c_str(), xmlrpc_port);


	//Muss hier logisch am Ende sein.
	ros::ServiceServer server = node->advertiseService("robotCommand", robotCommand);
	ROS_INFO("robotCommand service activated.");

	ros::spin();
}

bool ServerNode::robotCommand(UR_control::robotCommandRequest &request, UR_control::robotCommandResponse &response)
{
	std::string command;
	static const std::function<std::string(const std::string& command)> handleCommandRequestStandard([](const std::string& command)->std::string{return robotConnection->sendCommandToRobot(command);});
	std::function<std::string(const std::string& command)> handleCommandRequest(handleCommandRequestStandard);

	switch (request.command){
        case request.LOAD:
            command = std::string("load ").append(request.program);
			handleCommandRequest = &requestCommandLOAD;
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
			response.response = "Error: Incorrect command";
			ROS_ERROR("%s", response.response.c_str());
			return true;
	}

	ROS_INFO("Received command:\n%s", command.c_str());
	response.response = handleCommandRequest(command);

	return true;
}

std::string ServerNode::requestCommandLOAD(const std::string& command)
{
	std::string response;

	//gleich, d.h. request.program ist leer.
	if ( command.compare("load ") == 0 ) {
		if ( program_name.empty() ) {
			response = "Error: program name not provided.";
			ROS_ERROR("%s", response.c_str());
			return response;
		}

		std::string alteredCommand = command + program_name;
		response = robotConnection->sendCommandToRobot(alteredCommand);
		return response;
	}

	response = robotConnection->sendCommandToRobot(command);
	return response;
}

std::string ServerNode::requestCommandPLAY(const std::string& command)
{
	std::string response;
	std::string isRunningString = robotConnection->sendCommandToRobot("running");

	//weiteres Mal
	if( isRunningString.find("true") != std::string::npos ) {
		//XMLRPC Server benachrichtigen
		XmlRpc::XmlRpcValue pause_false(false); ////#pose.pause = True bedeutet PAUSE; False bedeutet PLAY
		XmlRpc::XmlRpcValue setPauseReturn;
		xmlRpcClient->execute("setPause", pause_false, setPauseReturn);

		response = "Robot is already running.";

		if ( xmlRpcClient->isFault() ) {
			response += "\nError: XMLRPC execution faulted.";
			ROS_ERROR("%s", response.c_str());
		} else {
			response += "\nNotified XMLRPC Server.";
		}

		return response;
	}

	// erstes Mal
	if ( isRunningString.find("false") != std::string::npos ) {
		response = robotConnection->sendCommandToRobot("play");
		return response;
	}
	
	//Fehler
	response = "Error: Robot message not interpretable: " + isRunningString;
	ROS_ERROR("%s", response.c_str());
	return response;
}


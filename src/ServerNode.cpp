#include "UR_control/ServerNode.h"
#include "ros/console.h"
#include <optional>

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
	std::optional<std::string> decodedCommand(decodeCommand(request));

	if( !decodedCommand.has_value() ) {
		ROS_ERROR("Incorrect command");
		response.response = "Incorrect command";
	} else {
		response.response = sendCommandToRobot(decodedCommand.value());
	}

	return true;
}

std::optional<std::string> ServerNode::decodeCommand(UR_control::robotCommandRequest& request)
{
	std::string commandName;

	switch (request.command){
        case request.LOAD:
            commandName = std::string("load ").append(request.program);
            break;
        case request.PLAY:
            commandName = std::string("play");
            break;
        case request.STOP:
            commandName = std::string("stop");
            break;
        case request.PAUSE:
            commandName = std::string("pause");
            break;
        case request.RUNNING:
            commandName = std::string("running");
            break;
        default:
			return std::nullopt;
	}

	ROS_INFO("Received command:\n%s", commandName.c_str());

	return make_optional(commandName);
}

std::string ServerNode::sendCommandToRobot(std::string& command)
{
	std::string alteredCommand(command);
	alteredCommand.append("\n"); //Anforderung vom UR Dashboard Server
	
	robotConnection->send(alteredCommand);

	std::string response = robotConnection->receive();
	ROS_INFO("Robot response:\n%s", response.c_str());

	return response;
}


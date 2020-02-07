#include "robot_control/ServerNode.h"

std::string ServerNode::program_name = "";

std::unique_ptr<RobotConnection> ServerNode::robotConnection = nullptr;

ServerNode::ServerNode(int argc, char **argv)
{
	ros::init(argc, argv, "robot_command_server");
	node = std::make_unique<ros::NodeHandle>();

	ros::NodeHandle nh_priv("~");
	bool roboter_ipSet = nh_priv.getParam("roboter_ip", roboter_ip);
	bool roboter_portSet = nh_priv.getParam("roboter_port", roboter_port);
	bool program_nameSet = nh_priv.getParam("program_name", program_name);

	if (!roboter_ipSet || !roboter_portSet || !program_nameSet)
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

bool ServerNode::robotCommand(robot_control::robotCommandRequest &request, robot_control::robotCommandResponse &response)
{
	std::string commandName;

	switch (request.command){
        case request.LOAD:
            commandName = std::string("load ").append(program_name);
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
        default:
            ROS_ERROR("Incorrect command");
            response.response = "Incorrect command";
            return true;
	}

	ROS_INFO("Received command:\n%s", commandName.c_str());

	commandName.append("\n"); //Anforderung vom UR Dashboard Server

	robotConnection->send(commandName);

	response.response = robotConnection->receive();
	ROS_INFO("Robot response:\n%s", response.response.c_str());

	return true;
}


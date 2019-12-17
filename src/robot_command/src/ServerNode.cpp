#include "robot_command/ServerNode.h"

std::string ServerNode::program_name = nullptr;
std::unique_ptr<Socket::ProtocolSimple> ServerNode::protocolSimple = nullptr;

ServerNode::ServerNode(int argc, char **argv)
{
	ros::init(argc, argv, "robot_command server");
	node = std::make_unique<ros::NodeHandle>();
	bool roboter_ipSet = node->getParam("roboter_ip", roboter_ip);
	bool roboter_portSet = node->getParam("roboter_port", roboter_port);
	bool program_nameSet = node->getParam("program_name", program_name);

	if (!roboter_ipSet || !roboter_portSet || !program_nameSet) {
		ROS_ERROR("Please set roboter_ipSet, roboter_portSet and program_nameSet in the .launch file");
		ros::shutdown();
	}
}

void ServerNode::start()
{
	ros::ServiceServer server = node->advertiseService("robot_command", robotCommand);
	ROS_INFO("robot_command service activated");

	Socket::ConnectSocket connectSocket(roboter_ip, roboter_port);
	protocolSimple = std::make_unique<Socket::ProtocolSimple>(connectSocket);

	ros::spin();
}

bool ServerNode::robotCommand(robot_command::robotCommandRequest& request, robot_command::robotCommandResponse& response)
{
	ROS_INFO("Request received.");
	
	std::string commandName;

	switch (request.command)
	{
	case request.LOAD:
		commandName = std::string("load ").append(program_name);
		break;
	case request.PLAY:
		commandName = "play";
		break;
	case request.STOP:
		commandName = "stop";
		break;
	case request.PAUSE:
		commandName = "pause";
		break;
	default:
		ROS_ERROR("Incorrect command");
		response.response = "Incorrect command";
		return true;
	}

	ROS_INFO("command: %s", commandName.c_str());

	protocolSimple->sendMessage("", commandName);

	std::string robot_response;
	protocolSimple->recvMessage(robot_response);

	response.response = robot_response;

	return true;
}


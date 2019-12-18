#include "robot_command/ServerNode.h"

std::string ServerNode::roboter_ip = "";
int ServerNode::roboter_port = -1;
std::string ServerNode::program_name = "";

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
	ros::ServiceServer server = node->advertiseService("ur5e_cmd", robotCommand);
	ROS_INFO("robot_command service activated");

	ros::spin();
}

bool ServerNode::robotCommand(robot_command::robotCommandRequest &request, robot_command::robotCommandResponse &response)
{
	ROS_INFO("Request received.");

	Socket::ConnectSocket connectSocket(roboter_ip, roboter_port);
	Socket::ProtocolSimple protocolSimple(connectSocket);

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

	//protocolSimple->sendMessage("", commandName);

	connectSocket.putMessageData(commandName.c_str(), commandName.size() + 1);
	connectSocket.putMessageClose();

	std::string robot_response;
	protocolSimple.recvMessage(robot_response);

	ROS_INFO("response:\n%s", robot_response.c_str());

	response.response = robot_response;

	return true;
}

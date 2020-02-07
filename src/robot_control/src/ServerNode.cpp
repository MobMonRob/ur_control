#include "robot_control/ServerNode.h"

std::string ServerNode::program_name = "";

std::unique_ptr<Socket::ConnectSocket> ServerNode::connectSocket = nullptr;

std::unique_ptr<Socket::ProtocolSimple> ServerNode::protocolSimple = nullptr;

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
	ROS_INFO("robotCommand service activated...");

////
	ROS_INFO("Connect to %s:%i", roboter_ip.c_str(), roboter_port);

	//Socket::ConnectSocket connectSocket(roboter_ip, roboter_port);

	connectSocket = std::make_unique<Socket::ConnectSocket>(roboter_ip, roboter_port);

	//Socket::ProtocolSimple protocolSimple(connectSocket);

	protocolSimple = std::make_unique<Socket::ProtocolSimple>(*connectSocket);
////

	ros::spin();
}

bool ServerNode::robotCommand(robot_control::robotCommandRequest &request, robot_control::robotCommandResponse &response)
{
	ROS_INFO("Request received.");
    
    //ROS_INFO("Try to connect ip=%s:%i", roboter_ip.c_str(), roboter_port);

	//Socket::ConnectSocket connectSocket(roboter_ip, roboter_port);
    
	//Socket::ProtocolSimple protocolSimple(connectSocket);

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

	ROS_INFO("command: %s", commandName.c_str());

	commandName.append("\n"); //Anforderung vom UR Dashboard Server

	connectSocket->putMessageData(commandName.c_str(), commandName.size());
	//connectSocket->putMessageClose();

	std::string robot_response;
	protocolSimple->recvMessage(robot_response);

	ROS_INFO("response:\n%s", robot_response.c_str());

	response.response = robot_response;

	return true;
}

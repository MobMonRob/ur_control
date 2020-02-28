#include "UR_control/ClientNode.h"

#include <string>

ClientNode::ClientNode(int argc, char **argv)
{
	ros::init(argc, argv, "robot_command_client");
	node = std::make_unique<ros::NodeHandle>();
	client = std::unique_ptr<ros::ServiceClient>(new ros::ServiceClient(node->serviceClient<UR_control::robotCommand>("robotCommand"))); //Stack->Heap per copy-constructor
}

void ClientNode::robotCommand(UR_control::robotCommandRequest &commandReq)
{
	std::string commandName;

	switch (commandReq.command)
	{
	case commandReq.LOAD:
		commandName = "LOAD";
		break;
	case commandReq.PLAY:
		commandName = "PLAY";
		break;
	case commandReq.STOP:
		commandName = "STOP";
		break;
	case commandReq.PAUSE:
		commandName = "PAUSE";
		break;
    case commandReq.RUNNING:
		commandName = "RUNNING";
		break;
	default:
		ROS_ERROR("Incorrect command");
		return;
	}

	ROS_INFO("Sent command:\n%s", commandName.c_str());

	UR_control::robotCommand robotCommand;
	robotCommand.request = commandReq;

	bool success = client->call(robotCommand);

	if (success)
	{
		ROS_INFO("Robot response:\n%s", robotCommand.response.response.c_str());
	}
	else
	{
		ROS_ERROR("Error between client and server.");
	}
}

void ClientNode::test()
{
	UR_control::robotCommandRequest request;

	while (ros::ok())
	{
		testOneCommand(request.LOAD);
		testOneCommand(request.PLAY);
		testOneCommand(request.STOP);
		testOneCommand(request.RUNNING);
	}
}

void ClientNode::testOneCommand(UR_control::robotCommandRequest::_command_type command)
{
		static ros::Rate loop_rate(0.25);

		UR_control::robotCommandRequest request;
		
		request.command = command;
		robotCommand(request);

		ros::spinOnce();
		loop_rate.sleep();
}


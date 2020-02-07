#include "robot_control/ClientNode.h"

#include <string>

ClientNode::ClientNode(int argc, char **argv)
{
	ros::init(argc, argv, "robot_command_client");
	node = std::make_unique<ros::NodeHandle>();
	client = std::unique_ptr<ros::ServiceClient>(new ros::ServiceClient(node->serviceClient<robot_control::robotCommand>("robotCommand"))); //Stack->Heap per copy-constructor
}

void ClientNode::robotCommand(robot_control::robotCommandRequest &commandReq)
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
	default:
		ROS_ERROR("Incorrect command");
		return;
	}

	ROS_INFO("Sent command:\n%s", commandName.c_str());

	robot_control::robotCommand robotCommand;
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
	ros::Rate loop_rate(0.25);

	robot_control::robotCommandRequest request;

	while (ros::ok())
	{
		request.command = request.LOAD;
		robotCommand(request);

		ros::spinOnce();
		loop_rate.sleep();

		request.command = request.PLAY;
		robotCommand(request);

		ros::spinOnce();
		loop_rate.sleep();

		request.command = request.STOP;
		robotCommand(request);
	}
}


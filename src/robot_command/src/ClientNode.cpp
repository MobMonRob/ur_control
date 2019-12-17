#include "robot_command/ClientNode.h"

#include <string>

ClientNode::ClientNode(int argc, char **argv)
{
	ros::init(argc, argv, "robot_command Client");
	node = std::make_unique<ros::NodeHandle>();
	client = std::unique_ptr<ros::ServiceClient>(new ros::ServiceClient(node->serviceClient<robot_command::robotCommand>("robot_command"))); //Stack->Heap per copy-constructor
}

void ClientNode::robotCommand(robot_command::robotCommandRequest &commandReq)
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

	ROS_INFO("Command: %s", commandName.c_str());

	robot_command::robotCommand robotCommand;
	robotCommand.request = commandReq;

	bool success = client->call(robotCommand);

	if (success)
	{
		ROS_INFO("%s", robotCommand.response.response.c_str());
	}
	else
	{
		ROS_ERROR("Error between client and server.");
	}
}

void ClientNode::test()
{
	ros::Rate loop_rate(0.25);

	robot_command::robotCommandRequest request;

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

#include "robot_command/ServerNode.h"

//std::unique_ptr<GpioManager> ServerNode::gpio = nullptr;

ServerNode::ServerNode(int argc, char **argv)
{
	ros::init(argc, argv, "robot_command Server");
	node = std::make_unique<ros::NodeHandle>();
	//gpio = std::make_unique<GpioManager>();
}

void ServerNode::start()
{
	ros::ServiceServer server = node->advertiseService("robot_command", robotCommand);
	ROS_INFO("robot_command Service aktiviert");
	ros::spin();
}

bool ServerNode::robotCommand(robot_command::robotCommandRequest& request, robot_command::robotCommandResponse& response)
{
	//res.success = gpio->writePin(req.pin, req.high);
	ROS_INFO("Request erhalten.");
	
	return true;
}



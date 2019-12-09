#include "robot_command/ServerNode.h"

ServerNode::ServerNode(int argc, char **argv)
{
	ros::init(argc, argv, "robot_command Server");
	node = std::make_unique<ros::NodeHandle>();
	bool roboter_ipSet = node->getParam("roboter_ip", roboter_ip);
	bool program_nameSet = node->getParam("program_name", program_name);

	if (roboter_ipSet || program_nameSet) {
		ROS_ERROR("Please set roboter_ipSet and program_nameSet in the .launch file");
		ros::shutdown();
	}
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


#include "robot_command/ClientNode.h"

ClientNode::ClientNode(int argc, char **argv)
{
	ros::init(argc, argv, "robot_command Client");
	node     = std::make_unique<ros::NodeHandle>();
	client   = std::unique_ptr<ros::ServiceClient>(new ros::ServiceClient(node->serviceClient<robot_command::robotCommand>("robot_command"))); //Stack->Heap per copy-constructor
}

void ClientNode::robotCommand(robot_command::robotCommandRequest::_command_type& command)
{
/*
  rospi::WritePin writePin;

  writePin.request.pin = pin;
  writePin.request.high = high;

	ROS_INFO("Pin: %d, High: %d", pin, high);

  bool success = client->call(writePin);

  if (success)
  {
		if (writePin.response.success)
		{
			ROS_INFO("Hat funktioniert.");
		}
		else
		{
  	  ROS_ERROR("Beim Server ist etwas schiefgelaufen.");
		}
  }
  else
  {
    ROS_ERROR("Zwischen Client und Server ist etwas schiefgelaufen.");
  }~
*/
}

void ClientNode::test()
{
	ros::Rate loop_rate(2);
	
	while (ros::ok())
	{
		//
	
		ros::spinOnce();
		loop_rate.sleep();
	}
}


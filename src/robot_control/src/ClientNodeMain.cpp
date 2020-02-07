#include "robot_control/ClientNode.h"

int main(int argc, char **argv)
{
	ClientNode clientNode(argc, argv);
	clientNode.test();

  return 0;
}


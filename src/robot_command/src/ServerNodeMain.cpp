#include "robot_command/ServerNode.h"

int main(int argc, char **argv)
{
	ServerNode serverNode(argc, argv);
	serverNode.start();
	return 0;
}


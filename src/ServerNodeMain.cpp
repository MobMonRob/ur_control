#include "UR_control/ServerNode.h"

int main(int argc, char **argv)
{
	ServerNode serverNode(argc, argv);
	serverNode.start();
	return 0;
}


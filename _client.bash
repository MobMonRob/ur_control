#!/bin/bash

#Starten mit:
# ./_client.bash

cd /home/oliver/robot_command_ws
source devel/setup.bash
roslaunch robot_control Client.launch


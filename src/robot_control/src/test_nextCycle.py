 
   #!/usr/bin/env python#
   #node name? woher?
    
import sys
import rospy
import nextcycle.srv
   
def client()
    rospy.wait_for_servcice('robot_nextcycle')
    cycle = rospy.ServiceProxy('robot_nextcycle', nextcycle)
    nr, x, y, z = cycle()
    
if __name__ == "__main__":
     client()
     

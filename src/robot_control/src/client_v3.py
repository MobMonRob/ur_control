
import xmlrpc.client

print('client started')
url = 'http://192.168.12.10:8000' # 'http://192.168.12.10:8000'
s = xmlrpc.client.ServerProxy(url, encoding="UTF-8")
boxSize = 0 # 0: big box, 1: small box
newIndex = 0


#print(s.getBoxSize())  # Returns 0 or 1

#print("The new index is: ", s.setIndex(boxSize, newIndex)) # Sets the index for the next pose
#print(s.nextRelativePose(boxSize))  # Returns the next pose

#value = True #pause flag: True = pause, False = play
#print("System pause is now: ", s.getPause()) #get State of the pause flag
#print("System pause is now: ", s.setPause(value)) #sets the State of the pause flag
#print("System pause is now: ", s.togglePause()) #toggles the State of the pause flag

#print("The new calibrationindex is: ", s.setCalibrationIndex(newIndex)) # Sets the index for the next calibrationPose
#print(s.nextCalibrationPose()) # Returns the next calibration pose

# Print list of available methods
print(s.system.listMethods())

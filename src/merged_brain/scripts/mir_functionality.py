#!/usr/bin/env python
import numpy as np
import rospy
from mir_interface.srv import *
from geometry_msgs.msg import Point

class Mir:

    def __init__(self, ip = None):
        self.ip = ip

        self.action_list = np.array(["goto", "get_gps", "free_slot", "add_obj", "remove_obj"])
        self.gps = np.array([None, None, None])
        #print("in init")

    def unit_action(self, request, parameters = np.array([])):
        if isinstance(request, type("")):
            #print("Action a string, ok")
            rospy.wait_for_service('sim/mir/get')
            r = get_actionRequest()
            r.action = request
            try:
                act = rospy.ServiceProxy('sim/mir/get', get_action)
                if request == "gps":
                    resp1 = act(r)
                    self.gps[0] = resp1.x
                    self.gps[1] = resp1.y
                    self.gps[2] = resp1.z
                    return self.gps
                elif request == "goto":
                    #resp1 = act(r)
                    #print(11)
                    self.unit_goto(parameters)
                    return True
                elif request == "free_slot":
                    resp1 = act(r)
                    if resp1.pos_num != -1: #  -1 = no free positions on the mir
                        gps_object = np.array([None, None, None])
                        self.gps[0] = resp1.x
                        self.gps[1] = resp1.y
                        self.gps[2] = resp1.z
                        return gps_object, resp1.pos_num
                elif request == "add_obj":
                    r.num = parameters[0]
                    r.obj_type = parameters[1]
                    resp1 = act(r)
                    return True
                elif request == "remove_obj":
                    r.num = parameters
                    resp1 = act(r)
                    return True
                elif request == "state":
                    resp1 = act(r)
                    return resp1.status
            except rospy.ServiceException as e:
                print ("Service call failed:", e)
        else:
            pass
            #print("Action not string, ok")

    def unit_goto(self, coordinates = np.array([])): # x, y = 0.0, z = 0.0, w = 1.0):
        rospy.wait_for_service('coppelia_interface/mir/goto')
        try:
            #print("here")
            #rospy.loginfo(self.ip, " Mir, action goto ", coordinates)
            goto = rospy.ServiceProxy('coppelia_interface/mir/goto', goto_point)
            if coordinates.size < 2:
                resp1 = goto(coordinates[0], 0.0, 0.0)
            else:
                resp1 = goto(coordinates[0], coordinates[1], 0.0)
            return resp1.success
        except rospy.ServiceException as e:
            print ("Service call failed:", e)



#a = Mir()
#a.unit_action(request="goto", parameters=np.array([0.001, 0.0001, 0.1]))

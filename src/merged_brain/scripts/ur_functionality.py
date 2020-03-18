#!/usr/bin/env python
import numpy as np
import rospy
from ur_move.srv import *


class Ur:

    def __init__(self, ip = None):
        self.ip = ip

        self.action_list = np.array(["goto", "get_gps", "find_obj", "pickup_obj", "putdown_obj"])
        self.gps = np.array([None, None, None])
        #print("in init")
    def unit_action(self, request, parameters = np.array([])):
        if isinstance(request, type("")):
            #print("Action a string, ok")
            rospy.wait_for_service('sim/ur5/get')
            r = get_actionRequest()
            r.action = request
            try:
                act = rospy.ServiceProxy('sim/ur5/get', get_action)
                if request == "gps":
                    resp1 = act(r)
                    self.gps[0] = resp1.x
                    self.gps[1] = resp1.y
                    self.gps[2] = resp1.z
                    return self.gps
                elif request == "goto":
                    resp1 = act(r)
                    self.unit_goto(parameters)
                    return True
                elif request == "find_obj":
                    print("finding object", parameters)
                    r.obj_type = parameters
                    resp1 = act(r)
                    print("object found")
                    obj_location = np.array([None, None, None])
                    obj_location[0] = resp1.x
                    obj_location[1] = resp1.y
                    obj_location[2] = resp1.z
                    return obj_location
                elif request == "suction_cup":
                    if parameters == 1:
                        r.action = "suction_cup_on"
                        resp1 = act(r)
                    else:
                        r.action = "suction_cup_off"
                        resp1 = act(r)
                    return True
                elif request == "pickup_obj":
                    self.pickup_obj(requested_obj=parameters[0])
                elif request == "putdown_obj":
                    self.putdown_obj(coordinates=parameters)
                elif request == "state":
                    resp1 = act(r)
                    return resp1.status
            except rospy.ServiceException as e:
                print ("Service call failed:", e)
        else:
            pass
            #print("Action not string, ok")

    def unit_goto(self, coordinates = np.array([])): # x, y = 0.0, z = 0.0, w = 1.0):
        rospy.wait_for_service('coppelia_interface/ur5/goto')
        try:
            rospy.loginfo("Called UR goto")
            goto = rospy.ServiceProxy('coppelia_interface/ur5/goto', goto_point)
            resp1 = goto(coordinates[0], coordinates[1], coordinates[2], 1.0)
            #rospy.loginfo("Called UR goto DONE!!")
            return resp1.success
        except rospy.ServiceException as e:
            print ("Service call failed:", e)
    def pickup_obj(self, requested_obj):
        suc = 1.0074e-01+3.0000e-02/3
        # Find object
        print("find object")
        object_coordinates = self.unit_action(request="find_obj", parameters=requested_obj)
        print("object found", object_coordinates)
        # Go above the object
        above_object = object_coordinates
        above_object[2] = above_object[2]+0.1+suc
        #print("going to object", above_object)
        self.unit_goto(above_object)
        self.unit_action(request="suction_cup", parameters=1)
        rospy.sleep(2)
        print("at object")
        pickup = object_coordinates
        pickup[2] = pickup[2]+0.025+suc
        self.unit_goto(pickup)

        rospy.sleep(5)
        self.unit_goto(above_object)

    def putdown_obj(self, coordinates = np.array([])):
        suc = 1.0074e-01+3.0000e-02/3
        # Go above the object
        above_object = coordinates
        above_object[2] = above_object[2]+0.1+suc
        #print("going to object", above_object)
        self.unit_goto(above_object)
        rospy.sleep(2)
        print("at object")
        putdownup = object_coordinates
        pickup[2] = pickup[2]+0.025+suc
        self.unit_goto(pickup)
        self.unit_action(request="suction_cup")
        rospy.sleep(2)
        self.unit_goto(above_object)
        #rospy.sleep(2)
        #self.unit_action(request="suction_cup")
        # Pick up object


#a = Ur()
#a.unit_action("pickup_obj", "A")

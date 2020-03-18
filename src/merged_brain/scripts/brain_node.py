#!/usr/bin/env python
import numpy as np
import rospy
from mir_functionality import Mir as Mir_unit
from ur_functionality import Ur as Ur5_unit
from merged_brain.msg import RecruitUnit


brain_nodes = np.array([])

class Brain_node:

    def __init__(self, id, parent = None, unit_type = None):
        self.children = np.array([])
        self.parent = parent
        self.id = id
        self.mode = "single"  # modes - single, brain, slave
        self.select_unit(unit_type)
        self.position = self.unit.unit_action(request="gps") # np.array([None, None, None]) #
        #self.unit_type = unit_type

        rospy.Subscriber("/brains/available", RecruitUnit,
            self.callback_recruit_unit)
        rospy.Subscriber("/brains/"+ self.id, RecruitUnit,
            self.callback_id)
        self.publisher_available = rospy.Publisher('/brains/available',
            RecruitUnit, queue_size=10)
    def select_unit(self, unit_type):
        self.unit_type = unit_type
        if unit_type == "ur":
            self.unit = Ur5_unit()

        elif unit_type == "mir":
            self.unit = Mir_unit()
        else:
            self.unit = [None, None]
        #return unit
    def add_parent(self, parent):
        self.parent = parent
    def add_child(self, child):
        self.children = np.append(self.children, child)
        child.add_parent(self)
        child.mode = "slave"
    def remove_parent(self):
        self.parent = None
    def remove_child(self, child):
        place = np.where(self.children == child)
        self.children = np.delete(self.children, place)
        child.remove_parent()
        child.change_mode()
        if not self.children.size:
            self.change_mode()
    def change_mode(self, mode = "single"):
        self.mode = mode
    def get_parent(self):
        return self.parent
    def type_check(self, check_type):
        checklist = np.array([])
        checklist = np.append(checklist, self.unit.action_list)
        checklist = np.append(checklist, self.unit_type)
        type_request = False
        for types in checklist:
            if check_type == types:
                type_request = True
                break
        return type_request
    def get_children(self, unit_type = None):
        if self.children.size == 0:
            if unit_type == None or self.unit_type == unit_type:
                return self
            else:
                child_list = np.array([])
        else:
            if unit_type == None or self.unit_type == unit_type:
                child_list = np.array([self])
            else:
                child_list = np.array([])
            for child in self.children:
                child_list = np.append(child_list, child.get_children(
                    unit_type=unit_type))
        return child_list
    def find_closest_unit(self, unit_type,
                        position=np.array([]), distance=99999.0):
        closest_distance = 9999.0

        type_request = self.type_check(unit_type)

        if position.size == 0:
            position = self.position
            dist = 999.0
        else:
            #dist = abs(position[1] - self.position[1])
            temp = 0
            for i in range(2): # only checks x y dist... 3 for check of x y z dist
                temp = temp + np.power(position[i] - self.position[i], 2)
            dist = np.sqrt(temp)
        if not self.children.any():
            if type_request:
                if dist < distance:
                    return self, dist
        else:
            if type_request:
                if dist < distance:
                    closest_child = self
                    closest_distance = dist
            for child in self.children:
                _child, _distance = child.find_closest_unit(
                    unit_type=unit_type,
                    position=position,
                    distance=closest_distance)
                if _distance < closest_distance:
                    closest_child = _child
                    closest_distance = _distance
            return closest_child, closest_distance
        return np.array([]), -1.0

    def callback_recruit_unit(self, data):
        if not(data.recruiter_id == self.id) and self.mode == "single":
            type_request = self.type_check(data.type)

            if type_request:
                pub = rospy.Publisher('/brains/' + data.recruiter_id,
                    RecruitUnit, queue_size=10)
                rospy.sleep(0.3)
                msg = RecruitUnit()
                msg.type = self.unit_type
                msg.recruiter_id = data.recruiter_id
                msg.sender_id = self.id
                msg.x = self.position[0]
                msg.y = self.position[1]
                msg.z = self.position[2]
                temp = np.power(data.x - self.position[0], 2) \
                    + np.power(data.y - self.position[1], 2)  \
                    + np.power(data.z - self.position[2], 2)
                dist = np.sqrt(temp)
                msg.distance = dist
                rospy.loginfo(str(self.id) + " responce to " + data.sender_id)
                pub.publish(msg)
        else:
            pass
    def callback_id(self, data):
        self.recruits = np.append(self.recruits,
            [[data.sender_id, data.distance]], axis = 0)
    def recruit_unit(self, type):
        self.recruits = np.empty((0, 2))
        if not (self.mode == "slave"):

            msg = RecruitUnit()
            msg.type = type
            msg.recruiter_id = self.id
            msg.sender_id = self.id
            msg.x = self.position[0]
            msg.y = self.position[1]
            msg.z = self.position[2]
            rospy.loginfo(str(self.id)+ " trying to recuirt type: " + type)
            self.publisher_available.publish(msg)
            rospy.sleep(1)
            if self.recruits.size:
                dist = 999.0
                for recruit_id, distance in self.recruits:
                    if float(distance) < dist:
                        dist = float(distance)
                        id = recruit_id
                self.add_child(brain_nodes[int(id)])
                self.change_mode("master")
                rospy.loginfo(str(self.id) + " has recuirted " + id)
            return brain_nodes[int(id)]
    def task_handler(self, input_file):
        _brain = self
        for action, parameter in input_file:
            if action == "goto_unit":
                action = "goto"
                if parameter == "pre_unit":
                    parameter = _brain.position
            if np.where(self.unit.action_list == action)[0].size > 0:
                _brain = self
            elif self.children.size != 0:
                brain, dist = self.find_closest_unit(action)
                if dist != -1.0:
                    _brain = brain
            else:
                rospy.loginfo(str(self.id) + " Recruiting unit for action " + action)
                #print("Recruiting")
                _brain = self.recruit_unit(action)
                #print("Recruited" + str(self.children.size))
            _brain.unit.unit_action(request=action, parameters=parameter)
        #for tasks, parameters in input_file:
            # check self or child should to task, best should do

            # If self or no child can do task recruit



            #self.unit.action_list

#a = Brain_node(id = "mir", unit_type = "mir")
#b = Brain_node(id = "b", unit_type = "mir")
#c = Brain_node(id = "c", unit_type = "ur")
#d = Brain_node(id = "d", unit_type = "mir")
#a.position = np.array([0,0,0])
#b.position = np.array([0,3,0])
#c.position = np.array([0,2,0])
#d.position = np.array([0,1,0])


def main():
    rospy.init_node('brains', anonymous=True)
    global brain_nodes
    brain_nodes = np.append(brain_nodes,
        Brain_node(id = str(brain_nodes.size), unit_type = "mir"))
    brain_nodes = np.append(brain_nodes,
        Brain_node(id = str(brain_nodes.size), unit_type = "ur"))
    #c = Brain_node(id = "3", unit_type = "ur")
    r = rospy.Rate(10)
    rospy.sleep(1)
    #a.position = np.array([2.0, 1.0, 0.0])
    #b.position = np.array([1.5, 1.0, 0.10])
    #c.position = np.array([1.0, 1.0, 1.0])
    rospy.loginfo("Brain node waiting for interfaces")
    rospy.wait_for_service('coppelia_interface/ur5/goto')
    rospy.loginfo("Brain node waiting for Coppelia")
    rospy.wait_for_service('sim/ur5/get')
    #print("Recruiting")
    #brain_nodes[0].recruit_unit("ur")
    #print("Recruiting DONE")
    input_file = np.empty((0,2))
    input_file = np.append(input_file, [["pickup_obj", "A"]], axis=0)
    input_file = np.append(input_file, [["goto_unit", "pre_unit"]], axis=0)
    input_file = np.append(input_file, [["putdown", "mir"]], axis=0)
    #print(input_file)
    brain_nodes[0].task_handler(input_file)
    #brain_nodes[0].unit.unit_action(request="goto", parameters=brain_nodes[0].children[0].position)
    #rospy.sleep(2)
    #brain_nodes[0].children[0].unit.unit_action(request="pickup_obj", parameters="A")
    #rospy.sleep(2)
    #brain_nodes[0].unit.unit_action(request="goto", parameters=brain_nodes[0].children[0].position)

    rospy.spin()
    #r.sleep()

    #while not rospy.is_shutdown():

    #    r.sleep()




if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException:
        pass
#a.add_child(b)
#a.add_child(c)
#b.add_child(d)

#print("-------------- Get children")
#print(a.get_children(unit_type="mir"))
#print("-------------- Find closest unit_type = mir")
#print(a.find_closest_unit(unit_type="mir"))
#print("-------------- Find closest unit_type = ur")
#print(a.find_closest_unit(unit_type="ur"))
'''
def find_closest_unit(self, unit_type,
                    position=np.array([]), distance=99999.0):
    closest_distance = 9999.0
    if position.size == 0:
        position = self.position
        dist = 999.0
    else:
        #dist = abs(position[1] - self.position[1])
        temp = 0
        for i in range(2): # only checks x y dist... 3 for check of x y z dist
            temp = temp + np.power(position[i] - self.position[i], 2)
        dist = np.sqrt(temp)
    if not self.children.any():
        if self.unit_type == unit_type:
            if dist < distance:
                return self, dist
    else:
        if self.unit_type == unit_type:
            if dist < distance:
                closest_child = self
                closest_distance = dist
        for child in self.children:
            _child, _distance = child.find_closest_unit(
                unit_type=unit_type,
                position=position,
                distance=closest_distance)
            if _distance < closest_distance:
                closest_child = _child
                closest_distance = _distance
        return closest_child, closest_distance
    return np.array([]), 99.0

'''

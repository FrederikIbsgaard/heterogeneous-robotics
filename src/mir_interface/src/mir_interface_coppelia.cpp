#include <sstream>
#include <ros/ros.h>
#include "mir_interface/action.h"
#include "mir_interface/goto_point.h"
#include "std_msgs/String.h"
#include "std_msgs/Float32MultiArray.h"

 class Coppelia_interface
 {
 public:
    ros::NodeHandle n;
    ros::Publisher pub_mir_action = n.advertise<std_msgs::String>("/sim/mir/action", 1000);
    ros::Publisher pub_mir_goto = n.advertise<std_msgs::Float32MultiArray>("/sim/mir/goto", 1000);

    virtual bool mir_action_service(mir_interface::action::Request &req,
                     mir_interface::action::Response &res)
    {
        std_msgs::String msg;
        msg.data = req.action;
        pub_mir_action.publish(msg);
        return true;
    }
    virtual bool mir_goto_service(mir_interface::goto_point::Request &req,
                        mir_interface::goto_point::Response &res)
    {
        std::vector<float> q = {req.x, req.y, req.z};
        std_msgs::Float32MultiArray msg;
        // set up dimensions
        msg.layout.dim.push_back(std_msgs::MultiArrayDimension());
        msg.layout.dim[0].size = q.size();
        msg.layout.dim[0].stride = 1;
        msg.layout.dim[0].label = "x"; // or whatever name you typically use to index vec1

        // copy in the data
        msg.data.clear();
        msg.data.insert(msg.data.end(), q.begin(), q.end());

        pub_mir_goto.publish(msg);
        return true;
    }
 };


int main(int argc, char**argv)
{
    ros::init(argc, argv, "mir_interface");
    ros::NodeHandle n;
    Coppelia_interface coppelia_interface;
    ros::ServiceServer service_action = n.advertiseService("coppelia_interface/mir/action", &Coppelia_interface::mir_action_service, &coppelia_interface);
    ros::ServiceServer service_goto = n.advertiseService("coppelia_interface/mir/goto", &Coppelia_interface::mir_goto_service, &coppelia_interface);

    ros::spin();

}

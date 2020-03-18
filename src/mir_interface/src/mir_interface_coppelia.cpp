#include <sstream>
#include <ros/ros.h>
#include "mir_interface/action.h"
#include "mir_interface/goto_point.h"
#include "std_msgs/String.h"
#include "std_msgs/Float32MultiArray.h"
#include <tf/transform_datatypes.h>
#include "mir_interface/get_action.h"
 class Coppelia_interface
 {
 public:
    ros::NodeHandle n;
    ros::Publisher pub_mir_action = n.advertise<std_msgs::String>("/sim/mir/action", 1000);
    ros::Publisher pub_mir_goto = n.advertise<std_msgs::Float32MultiArray>("/sim/mir/goto", 1000);
    ros::ServiceClient client_mir_get_action = n.serviceClient<mir_interface::get_action>("/sim/mir/get");

    geometry_msgs::Point gps;
    bool received_msg;

    virtual bool mir_action_service(mir_interface::action::Request &req,
                     mir_interface::action::Response &res)
    {
        mir_interface::get_action srv;
        srv.request.action = req.action;
        client_mir_get_action.call(srv);
        res.x = srv.response.x;
        res.y = srv.response.y;
        res.z = srv.response.z;

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

    virtual void sim_gps_callback(geometry_msgs::Point msg)
    {
        ROS_INFO("Saving GPS");
        gps.x = msg.x;
        gps.y = msg.y;
        gps.z = msg.z;
        received_msg = true;
    }
 };


int main(int argc, char**argv)
{
    ros::init(argc, argv, "mir_interface");
    ros::NodeHandle n;
    ros::AsyncSpinner spinner(2);
    spinner.start();
    Coppelia_interface coppelia_interface;
    ros::ServiceServer service_action = n.advertiseService("coppelia_interface/mir/action", &Coppelia_interface::mir_action_service, &coppelia_interface);
    ros::ServiceServer service_goto = n.advertiseService("coppelia_interface/mir/goto", &Coppelia_interface::mir_goto_service, &coppelia_interface);

    ros::Subscriber sub = n.subscribe("/sim/mir/gps", 1000, &Coppelia_interface::sim_gps_callback, &coppelia_interface);

    ros::waitForShutdown();

}

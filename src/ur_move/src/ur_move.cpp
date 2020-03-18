
#include "moveit_interface.cpp"
#include <sstream>
#include <ros/ros.h>
#include "ur_move/q.h"
#include "ur_move/goto_point.h"
#include "ur_move/action.h"
#include "std_msgs/String.h"
#include "ur_move/get_action.h"

class Coppelia_interface
{
public:
    ros::NodeHandle n;
    ros::Publisher pub_ur5_action = n.advertise<std_msgs::String>("/sim/ur5/action", 1000);
    ros::ServiceClient client = n.serviceClient<ur_move::q>("coppelia_interface/ur5/move_to_joint");
    ros::ServiceClient client_ur5_get_action = n.serviceClient<ur_move::get_action>("/sim/ur5/get");

    std::vector<double> new_position;
    Moveit_interface moveit_interface;

    geometry_msgs::Point gps;
    bool received_msg;

    Coppelia_interface()
    {
        moveit_interface.initialize_robot();
        new_position = moveit_interface.get_joint_values();
        move_coppeliaSim(new_position);
    }
    virtual bool action_service(ur_move::action::Request &req,
                     ur_move::action::Response &res)
    {


        if (req.action == "home")
        {
            std_msgs::String msg;
            msg.data = req.action;
            moveit_interface.home();
            new_position = moveit_interface.get_joint_values();
            move_coppeliaSim(new_position);
            pub_ur5_action.publish(msg);
        } else if (req.action == "gps")
        {
            ur_move::get_action srv;
            srv.request.action = req.action;
            client_ur5_get_action.call(srv);
            res.x = srv.response.x;
            res.y = srv.response.y;
            res.z = srv.response.z;
        }


        return true;
    }
    void move_coppeliaSim(std::vector<double> &new_pos)
    {
        ur_move::q srv;
        srv.request.q1 = new_pos[0];
        srv.request.q2 = new_pos[1];
        srv.request.q3 = new_pos[2];
        srv.request.q4 = new_pos[3];
        srv.request.q5 = new_pos[4];
        srv.request.q6 = new_pos[5];
        client.call(srv);
    }
    void move_to(geometry_msgs::Pose &target)
    {
        moveit_interface.rviz_move(target);
        new_position = moveit_interface.get_joint_values();
        move_coppeliaSim(new_position);
    }
    virtual bool goto_point(ur_move::goto_point::Request &req,
                        ur_move::goto_point::Response &res) // can måske slettes
    {
        geometry_msgs::Pose target;
        target.orientation.w = req.w;
        target.position.x = req.x;
        target.position.y = req.y;
        target.position.z = req.z;
        move_to(target);
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
    ros::init(argc, argv, "ur_interface");
    ros::NodeHandle n;
    ros::AsyncSpinner spinner(2);
    spinner.start();

    //Coppelia_interface coppelia_interface;

    Coppelia_interface coppelia_interface;

    ros::ServiceServer service_action = n.advertiseService("coppelia_interface/ur5/action", &Coppelia_interface::action_service, &coppelia_interface);
    ros::ServiceServer service = n.advertiseService("coppelia_interface/ur5/goto", &Coppelia_interface::goto_point, &coppelia_interface);

    while(ros::master::check() && ros::ok());
    //ros::spin();
    //return EXIT_SUCCESS;

    return 0;

}

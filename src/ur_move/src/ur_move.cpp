
#include "moveit_interface.cpp"
#include <sstream>
#include <ros/ros.h>
#include "ur_move/q.h"
#include "ur_move/goto_point.h"
#include "ur_move/action.h"
#include "std_msgs/String.h"


class Service_class
{
public:
    ros::NodeHandle n;
    ros::Publisher pub_ur5_action = n.advertise<std_msgs::String>("/sim/ur5/action", 1000);
    ros::ServiceClient client = n.serviceClient<ur_move::q>("coppelia_interface/ur5/move_to_joint");
    std::vector<double> new_position;
    Moveit_interface moveit_interface;
    Service_class()
    {
        moveit_interface.initialize_robot();
        new_position = moveit_interface.get_joint_values();
        move_coppeliaSim(new_position);
    }
    virtual bool action_service(ur_move::action::Request &req,
                     ur_move::action::Response &res)
    {
        std_msgs::String msg;
        msg.data = req.action;
        if (req.action == "home")
        {
            moveit_interface.home();
            new_position = moveit_interface.get_joint_values();
            move_coppeliaSim(new_position);
        }
        pub_ur5_action.publish(msg);
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
                        ur_move::goto_point::Response &res)
    {
        geometry_msgs::Pose target;
        target.orientation.w = req.w;
        target.position.x = req.x;
        target.position.y = req.y;
        target.position.z = req.z;
        move_to(target);
        return true;
    }
};




int main(int argc, char**argv)
{
    ros::init(argc, argv, "ur_interface");
    ros::NodeHandle node;
    ros::AsyncSpinner spinner(2);
    spinner.start();

    //Coppelia_interface coppelia_interface;

    Service_class service_class;

    ros::ServiceServer service_action = node.advertiseService("coppelia_interface/ur5/action", &Service_class::action_service, &service_class);
    ros::ServiceServer service = node.advertiseService("coppelia_interface/ur5/goto_point", &Service_class::goto_point, &service_class);

    while(ros::master::check() && ros::ok());
    //ros::spin();
    //return EXIT_SUCCESS;

    return 0;

}

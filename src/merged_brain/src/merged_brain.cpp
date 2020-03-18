#include <sstream>
#include <ros/ros.h>

#include "mir_interface/action.h"
#include "mir_interface/goto_point.h"

#include "ur_move/q.h"
#include "ur_move/goto_point.h"
#include "ur_move/action.h"

#include "std_msgs/String.h"
#include "std_msgs/Float32MultiArray.h"


 class merged_brain
 {
 public:
    ros::NodeHandle n;
    ros::ServiceClient ur_move_action_client = n.serviceClient<ur_move::action>("coppelia_interface/ur5/action");
    ros::ServiceClient ur_move_goto_point_client = n.serviceClient<ur_move::goto_point>("coppelia_interface/ur5/goto_point");

    ros::ServiceClient mir_interface_action_client = n.serviceClient<mir_interface::action>("coppelia_interface/mir/action");
    ros::ServiceClient mir_interface_goto_point_client = n.serviceClient<mir_interface::goto_point>("coppelia_interface/mir/goto");

    void ur5_action(std::string &action)
    {
        ur_move::action srv;
        srv.request.x = action;
        client.call(srv);
    }

    void ur5_goto_point(std::vector<double> &new_pos)
    {
        ur_move::goto_point srv;
        srv.request.x = new_pos[0];
        srv.request.y = new_pos[1];
        srv.request.z = new_pos[2];
        client.call(srv);
    }
    void mir_action(std::string &action)
    {
        ur_move::action srv;
        srv.request.x = action;
        std::string = client.call(srv);
    }

    void mir_goto_point(std::vector<double> &new_pos)
    {
        ur_move::goto_point srv;
        srv.request.x = new_pos[0];
        srv.request.y = new_pos[1];
        srv.request.z = new_pos[2];
        client.call(srv);
    }

 };


int main(int argc, char**argv)
{
    ros::init(argc, argv, "merged_brain");

    //Robot_interface robot_interface;

    ros::spin();

}

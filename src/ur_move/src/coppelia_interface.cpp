#include <sstream>
#include <ros/ros.h>
#include "ur_move/q.h"
#include "std_msgs/Float32MultiArray.h"

 class Coppelia_interface
 {
 public:
     ros::NodeHandle na;
     ros::Publisher pub = na.advertise<std_msgs::Float32MultiArray>("/sim/ur5/move", 1000);

     virtual bool ur_move_service(ur_move::q::Request &req,
                         ur_move::q::Response &res)
     {
         std::vector<float> q = {req.q1, req.q2, req.q3, req.q4, req.q5, req.q6};
         std_msgs::Float32MultiArray msg;
         // set up dimensions
         msg.layout.dim.push_back(std_msgs::MultiArrayDimension());
         msg.layout.dim[0].size = q.size();
         msg.layout.dim[0].stride = 1;
         msg.layout.dim[0].label = "x"; // or whatever name you typically use to index vec1

         // copy in the data
         msg.data.clear();
         //msg.data.insert(msg.data.end(), q.begin(), q.end());
         msg.data.insert(msg.data.end(), q.begin(), q.end());
         pub.publish(msg);
         return true;
     }
 };


int main(int argc, char**argv)
{
    ros::init(argc, argv, "coppelia_interface");
    ros::NodeHandle n;

    Coppelia_interface coppelia_interface;
    ros::ServiceServer service = n.advertiseService("coppelia_interface/ur5/move_to_joint", &Coppelia_interface::ur_move_service, &coppelia_interface);

    ros::spin();

}

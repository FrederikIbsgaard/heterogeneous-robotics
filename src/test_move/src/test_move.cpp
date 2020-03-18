#include <ros/ros.h>
#include "std_msgs/Float32MultiArray.h"
#include <sstream>

/*
void publishQ(float q[], ros::Publisher pub) {
    std_msgs::Int16MultiArray msg;
    msg.data.clear();
    for(unsigned int i = 0; i < 6; i++){
            msg.data.push_back(q[i]);
    }
    pub.publish(msg);
}
*/

int main(int argc, char **argv) {
    ros::init(argc, argv, "test_move");
    ros::NodeHandle n;
    ros::Publisher test_move = n.advertise<std_msgs::Float32MultiArray>("/UR5_move", 1000);
    ros::Rate loop_rate(10);

    std::vector<float> q = {40.0,90.0,50.0,10.0,-90.0,90.0};

    //float q[6] = {90.0,90.0,90.0,90.0,90.0,90.0};
    std_msgs::Float32MultiArray msg;
    // set up dimensions
    msg.layout.dim.push_back(std_msgs::MultiArrayDimension());
    msg.layout.dim[0].size = q.size();
    msg.layout.dim[0].stride = 1;
    msg.layout.dim[0].label = "x"; // or whatever name you typically use to index vec1

    // copy in the data
    msg.data.clear();
    msg.data.insert(msg.data.end(), q.begin(), q.end());
    //msg = {90.0,90.0,90.0,90.0,90.0,90.0};



    while (ros::ok())
    {
        test_move.publish(msg);
        ROS_INFO("Message send");
        ros::spinOnce();
        loop_rate.sleep();


    }
    return 0;
}

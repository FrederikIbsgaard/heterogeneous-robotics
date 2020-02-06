#include <ros/ros.h>
#include "std_msgs/Float32MultiArray.h"
#include "ur_move/q.h"
#include <sstream>


class service_class
{
public:
    ros::NodeHandle na;
    ros::Publisher pub = na.advertise<std_msgs::Float32MultiArray>("/UR5_move", 1000);

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
        msg.data.insert(msg.data.end(), q.begin(), q.end());
        pub.publish(msg);
        return true;
    }
};



int main(int argc, char**argv)
{
    ros::init(argc, argv, "ur_move");
    ros::NodeHandle n;

    service_class a;
    ros::Rate loop_rate(10);

    ros::ServiceServer service = n.advertiseService("ur_move_service", &service_class::ur_move_service, &a);


    ros::spin();

    return 0;

}

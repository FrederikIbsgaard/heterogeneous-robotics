#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>

#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/CollisionObject.h>

#include <moveit_visual_tools/moveit_visual_tools.h>

#include <tf/transform_datatypes.h>

class Moveit_interface
{

public:
    const std::string PLANNING_GROUP = "manipulator";
    const robot_state::JointModelGroup* joint_model_group;
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
    moveit::planning_interface::MoveGroupInterface::Plan my_plan;
    std::vector<double> joint_group_positions;

    bool initialize_robot()
    {
        moveit::planning_interface::MoveGroupInterface move_group(PLANNING_GROUP);
        //------------------------------------------
        moveit_msgs::CollisionObject collision_object;
        collision_object.header.frame_id = move_group.getPlanningFrame();
        collision_object.id = "box1";
        shape_msgs::SolidPrimitive primitive;
        primitive.type = primitive.BOX;
        primitive.dimensions.resize(3);
        primitive.dimensions[0] = 2.4;
        primitive.dimensions[1] = 0.886;
        primitive.dimensions[2] = 0.953;


        geometry_msgs::Pose box_pose;
        box_pose.orientation.w = 1.0;
        box_pose.position.x = 0.0;
        box_pose.position.y = 0.0;
        box_pose.position.z = 0.4704; //0.4704

        collision_object.primitives.push_back(primitive);
        collision_object.primitive_poses.push_back(box_pose);
        collision_object.operation = collision_object.ADD;

        std::vector<moveit_msgs::CollisionObject> collision_objects;
        collision_objects.push_back(collision_object);
        planning_scene_interface.addCollisionObjects(collision_objects);
        //------------------------------------------
        home();
        return true;
    }
    bool home()
    {
        moveit::planning_interface::MoveGroupInterface move_group(PLANNING_GROUP);
        moveit::core::RobotStatePtr current_state = move_group.getCurrentState();
        joint_model_group = move_group.getCurrentState()->getJointModelGroup(PLANNING_GROUP);
        //std::vector<double> joint_group_positions;
        current_state->copyJointGroupPositions(joint_model_group, joint_group_positions);
        joint_group_positions[0] = 0;  // radians
        joint_group_positions[1] = -1.5707;  // radians
        joint_group_positions[2] = 0;  // radians
        joint_group_positions[3] = -1.5707;  // radians
        joint_group_positions[4] = 0;  // radians
        joint_group_positions[5] = 0;  // radians
        move_group.setJointValueTarget(joint_group_positions);


        bool success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

        move_group.move();
        return true;
    }
    void rviz_move(geometry_msgs::Pose &target)
    {
        moveit::planning_interface::MoveGroupInterface move_group(PLANNING_GROUP);
        moveit::core::RobotStatePtr current_state = move_group.getCurrentState();
        joint_model_group = move_group.getCurrentState()->getJointModelGroup(PLANNING_GROUP);


        tf::Quaternion q;

        q.setEulerZYX(0.0,1.5707,0.0);
        geometry_msgs::Pose target_pose1;
        target_pose1.position.x = target.position.x;
        target_pose1.position.y = target.position.y;
        target_pose1.position.z = target.position.z;
        target_pose1.orientation.x = q.x();
        target_pose1.orientation.y = q.y();
        target_pose1.orientation.z = q.z();
        target_pose1.orientation.w = q.w();
        move_group.setPoseTarget(target_pose1);




        bool success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
        move_group.setPoseTarget(target_pose1);
        move_group.move();

        //sleep(0.5);

    }
    std::vector<double> get_joint_values()
    {
        moveit::planning_interface::MoveGroupInterface move_group(PLANNING_GROUP);
        moveit::core::RobotStatePtr current_state = move_group.getCurrentState();
        joint_model_group = move_group.getCurrentState()->getJointModelGroup(PLANNING_GROUP);


        current_state = move_group.getCurrentState();
        //std::vector<double> joint_group_positions;
        current_state->copyJointGroupPositions(joint_model_group, joint_group_positions);
        return joint_group_positions;
    }

private:

};

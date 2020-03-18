cmake_minimum_required(VERSION 2.8.3)
project(ur_move)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation

  moveit_core
  moveit_visual_tools
  moveit_ros_planning
  moveit_ros_planning_interface
  moveit_ros_perception
  pluginlib
  geometric_shapes
  pcl_ros
  pcl_conversions
  rosbag
  tf2_ros
  tf2_eigen
  tf2_geometry_msgs
)

find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED system filesystem date_time thread)

## Generate services in the 'srv' folder
add_service_files(
    FILES
    action.srv
    q.srv
    goto_point.srv
    get_action.srv
)

## Generate added messages and services
generate_messages(
    DEPENDENCIES
    std_msgs
)


## Declare a catkin package
catkin_package(
    CATKIN_DEPENDS
        message_runtime
        moveit_core
        moveit_visual_tools
        moveit_ros_planning_interface
        interactive_markers
        tf2_geometry_msgs
    DEPENDS
        EIGEN3
)

## Build test_move
include_directories(include ${catkin_INCLUDE_DIRS}
                                ${THIS_PACKAGE_INCLUDE_DIRS}
                                ${catkin_INCLUDE_DIRS}
                                ${Boost_INCLUDE_DIR}
                                ${EIGEN3_INCLUDE_DIRS})

add_executable(ur_move src/ur_move.cpp)
target_link_libraries(ur_move ${catkin_LIBRARIES})
add_dependencies(ur_move ${catkin_LIBRARIES} ur_move_generate_messages_cpp ${PROJECT_NAME}_gencpp)

add_executable(coppelia_interface src/coppelia_interface.cpp)
target_link_libraries(coppelia_interface ${catkin_LIBRARIES})
add_dependencies(coppelia_interface ${catkin_LIBRARIES} coppelia_interface_generate_messages_cpp ${PROJECT_NAME}_gencpp)

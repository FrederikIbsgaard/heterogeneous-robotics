cmake_minimum_required(VERSION 3.10.2)
project(test_move)

add_compile_options(-std=c++11)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS
   roscpp
   rospy
   std_msgs
   message_generation
)

## Declare ROS messages and services
#add_message_files(
#  FILES
#  Num.msg
#)
#add_service_files(
#  FILES
#  AddTwoInts.srv
#)

## Generate added messages and services
#generate_messages(
#  DEPENDENCIES
#  std_msgs
#)

## Declare a catkin package
catkin_package(
CATKIN_DEPENDS message_runtime
)

## Build test_move
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(test_move src/test_move.cpp)
target_link_libraries(test_move ${catkin_LIBRARIES})
#add_dependencies(test_move ${catkin_LIBRARIES})

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
)


## Generate services in the 'srv' folder
add_service_files(
    FILES
    q.srv
)

## Generate added messages and services
generate_messages(DEPENDENCIES std_msgs)


## Declare a catkin package
catkin_package(
CATKIN_DEPENDS message_runtime
)

## Build test_move
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(ur_move src/ur_move.cpp)
target_link_libraries(ur_move ${catkin_LIBRARIES})
#add_dependencies(ur_move ${catkin_LIBRARIES})

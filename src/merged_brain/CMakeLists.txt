cmake_minimum_required(VERSION 2.8.3)
project(merged_brain)

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

find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED system filesystem date_time thread)

## Generate services in the 'srv' folder
add_service_files(
    FILES
    get_action.srv
    ##goto_point.srv
)
add_message_files(
  FILES
  RecruitUnit.msg
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
)

## Build test_move
include_directories(include ${catkin_INCLUDE_DIRS}
                                ${THIS_PACKAGE_INCLUDE_DIRS}
                                ${catkin_INCLUDE_DIRS}
                                ${Boost_INCLUDE_DIR})

#add_executable(merged_brain src/merged_brain.cpp)
#target_link_libraries(merged_brain ${catkin_LIBRARIES})
#add_dependencies(merged_brain ${catkin_LIBRARIES} merged_brain_generate_messages_cpp)

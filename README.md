# Heterogeneous Robotics
Download CoppeliaRobotics v.4.0 EDU and then export the path to the folder.

Write the following in the Linux terminal.

```
cd
echo 'export COPPELIASIM_ROOT_DIR="~/path/to/coppeliaSim/folder"' >> .bashrc
source .bashrc
```

## Install QT_5.12. Takes a ton of time

```
git clone git://code.qt.io/qt/qt5.git
cd qt5
git checkout v5.12.0
perl init-repository
../qt5/configure -developer-build -opensource -nomake examples -nomake tests
make
```

Remeber to run the CoppeliaRobotics simulation once.

```
cd $COPPELIASIM_ROOT_DIR
./coppeliaSim.sh
```

Copy the "libsimExtROSInterface.so" from the folder "compiledRosPlugins" to the root of the CoppeliaRobotics folder.

## Installing the ROS CoppeliaRobotics interface
[ROS](http://www.coppeliarobotics.com/helpFiles/en/ros1Tutorial.htm) guide.

Submodule for the ros interface, [sim_ros_interface](https://github.com/CoppeliaRobotics/simExtROSInterface).

Submodule for the ros interface example, [ros_bubble_rob](https://github.com/CoppeliaRobotics/ros_bubble_rob).

```
git submodule add  https://github.com/CoppeliaRobotics/simExtROSInterface.git sim_ros_interface
git submodule add  https://github.com/CoppeliaRobotics/ros_bubble_rob
git submodule update --recursive

catkin build --cmake-args -DCMAKE_BUILD_TYPE=Release -DLIBPLUGIN_DIR=$COPPELIASIM_ROOT_DIR/programming/libPlugin
```

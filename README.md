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

## Installing the ROS CoppeliaRobotics interface

..* Copy the "libsimExtROSInterface.so" from the folder "compiledRosPlugins" to the root of the CoppeliaRobotics folder.

[//]: # ### Not needed for the newest version of CoppeliaRobotics, Only need for V-REP
[//]: # [ROS](http://www.coppeliarobotics.com/helpFiles/en/ros1Tutorial.htm) guide.

[//]: # Submodule for the ros interface, [sim_ros_interface](https://github.com/CoppeliaRobotics/simExtROSInterface).

[//]: # Submodule for the ros interface example, [ros_bubble_rob](https://github.com/CoppeliaRobotics/ros_bubble_rob).

[//]: # ```
[//]: # git submodule add  https://github.com/CoppeliaRobotics/simExtROSInterface.git sim_ros_interface
[//]: # git submodule add  https://github.com/CoppeliaRobotics/ros_bubble_rob
[//]: # git submodule update --recursive

[//]: # catkin build --cmake-args -DCMAKE_BUILD_TYPE=Release -DLIBPLUGIN_DIR=$COPPELIASIM_ROOT_DIR/programming/libPlugin
[//]: # ```
    
## UR interface
### ur_rtde
Installation of [ros_bubble_rob](https://sdurobotics.gitlab.io/ur_rtde/installation/installation.html#linux-ubuntu).
Dependencies.
```
sudo apt-get install libboost-all-dev
pip3 install --user pybind11
```
Python.
```
pip3 install --user ur_rtde
```
C++.
```
git clone https://gitlab.com/sdurobotics/ur_rtde.git
cd ur_rtde
mkdir build
cd build
cmake ..
make
sudo make install
```

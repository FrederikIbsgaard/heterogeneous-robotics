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

Copy the "libsimExtROSInterface.so" from the folder "compiledRosPlugins" to the root of the CoppeliaRobotics folder.
    
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

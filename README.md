# heterogeneous-robotics
Download CoppeliaRobotics v.4.0 EDU and then export the path to the folder.

```
    export COPPELIASIM_ROOT_DIR="$HOME/CoppeliaSim_Edu_V4_0_0_Ubuntu18_04"
```

Submodule for the ros interface, sim_ros_interface, [simExtROSInterface](https://github.com/CoppeliaRobotics/simExtROSInterface).

Submodule for the ros interface example , [ros_bubble_rob](https://github.com/CoppeliaRobotics/ros_bubble_rob).

```
    git submodule add  https://github.com/CoppeliaRobotics/simExtROSInterface.git sim_ros_interface
    git submodule add  https://github.com/CoppeliaRobotics/ros_bubble_rob
    git submodule update --recursive
```



# Install QT_5.12. Takes a ton of time

```
    git clone git://code.qt.io/qt/qt5.git
    cd qt5
    git checkout v5.12.0
    perl init-repository
    ../qt5/configure -developer-build -opensource -nomake examples -nomake tests
    make
```


catkin build --cmake-args -DCMAKE_BUILD_TYPE=Release -DLIBPLUGIN_DIR=$COPPELIASIM_ROOT_DIR/programming/libPlugin

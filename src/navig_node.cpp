/** MIT License
Copyright (c) 2017 Miguel Maestre Trueba
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 *@copyright Copyright 2017 Miguel Maestre Trueba
 *@file navig_node.cpp
 *@author Miguel Maestre Trueba
 *@brief Navigation node where all the main functions of the robot behavious happen.
 */

#include <string>
#include <cstdlib>
#include "ros/ros.h"
#include "ros/console.h"
#include "robot.hpp"
#include "traffic_sign_recognition/sign.h"


/**
 *@brief Function main that runs the main algorithm of the robot behavior.
 *@brief It reads the messages published by the vision node using a ROS subscriber.
 *@brief Depending on what type of sign has been recognized, different actions will be executed.
 *@brief When the area of the detection is big enough and the sign classified, publish Twist messages to the robot.
 *@param argc is the number of arguments.
 *@param argv is the arguments characters array.
 *@return 0
 */
int main(int argc, char **argv) {
    // Node creation
    ros::init(argc, argv, "robot");
    ros::NodeHandle n;

    // Initializations
    robot turtle;
    geometry_msgs::Twist velocity;

    // Custom message topic Subscriber
    ros::Subscriber sub = n.subscribe("/traffic",
        1, &robot::signCallback, &turtle);

    // Velocity commands Publisher
    ros::Publisher pub = n.advertise<geometry_msgs::Twist>
        ("/cmd_vel_mux/input/teleop", 1000);

    // Main algorithm starts
    ros::Rate loop_rate(10);
    while (ros::ok()) {
        // Spin only when robot is not doing an action related traffic sign
        if (turtle.flag == false)
            ros::spinOnce();

        // Publish velocity depending on type of sign read in the message
        turtle.command(velocity, pub, loop_rate);

        loop_rate.sleep();
    }
    return 0;
}


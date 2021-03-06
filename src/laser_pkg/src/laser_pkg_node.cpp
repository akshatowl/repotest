#include<iostream>
#include <ros/ros.h>

#include <sensor_msgs/LaserScan.h>
#include <std_msgs/Float32.h>

void laser_dist_callback(const sensor_msgs::LaserScan::ConstPtr& msgs){
    double dist_left=msgs->ranges[0];
    double dist_right=msgs->ranges[1080];
    ROS_INFO("left: %f, right: %f", dist_left,dist_right);
}



int main(int argc, char **argv)
{

//Pointer to Laser Scanner Data
    sensor_msgs::LaserScanConstPtr dist;

//PID Variables
    double kp=2.0,ki=0.1,kd=1.0,dt=0.20;
    double error=0.0,error_new,p_error,i_error,d_error;
    double inp;
    std_msgs::Float32 basespeed,pubspeed;
    basespeed.data=2.0;
    

    ros::init(argc,argv,"scan");
    ros::NodeHandle nodes;
    ros::Subscriber sub_laser= nodes.subscribe("/ricky/laser/scan",5,laser_dist_callback);
    ros::Publisher left_wheel=nodes.advertise<std_msgs::Float32>("ricky/left_wheel_controller/command",5);
    ros::Publisher right_wheel=nodes.advertise<std_msgs::Float32>("ricky/right_wheel_controller/command",5);

error_new=(dist->ranges[0])-(dist->ranges[1080]);

p_error=kp*error;
i_error+=ki*error;
d_error=(error_new-error)/dt;

error=error_new;

if(i_error>5.0) //Basic Integral Clamp
{
    i_error=5.0;
}
else if(i_error<-5.0)
{
    i_error=-5.0;
}

inp=p_error+i_error+d_error;
pubspeed.data=basespeed.data-inp;
left_wheel.publish(pubspeed);
pubspeed.data=basespeed.data+inp;
right_wheel.publish(pubspeed);

//ROS_INFO("%f", basespeed-inp);
//ROS_INFO("%f", basespeed+inp);

ros::spin();
}
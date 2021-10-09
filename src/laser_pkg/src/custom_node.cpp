#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <std_msgs/Float32.h>
#include <geometry_msgs/Twist.h>
#include <unistd.h>

ros::Publisher pub;

void laser_callback(const sensor_msgs::LaserScan::ConstPtr& msgs)
{
    double dist_left=msgs->ranges[0];
    double dist_right=msgs->ranges[180];
    ROS_INFO("left: %f, right: %f", dist_left, dist_right);

    geometry_msgs::Twist pub_vel;
    pub_vel.linear.x=-5;

    pub.publish(pub_vel);
    usleep(10);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "custom_node");
    ros::NodeHandle nh;
    pub=nh.advertise<geometry_msgs::Twist>("/cmd_vel", 100);
    ros::Subscriber sub_laser= nh.subscribe("/ricky/laser/scan", 5, laser_callback);
    ros::spin();
    return 0;
}

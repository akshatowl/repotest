#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <std_msgs/Float32.h>
#include <geometry_msgs/Twist.h>
#include <unistd.h>

double kp=2.0,kd=0.07;
class laser_node
{
    ros::Publisher pub;
    ros::Subscriber sub_laser;
    sensor_msgs::LaserScan msg;
    geometry_msgs::Twist move;
    
public:
    void loop_function();
    // void callback(const sensor_msgs::LaserScan::ConstPtr &msg);

    laser_node(int rate)
    {
        //Node handler inside constructor
        ros::NodeHandle nh;
             
        this->sub_laser= nh.subscribe("/ricky/laser/scan", rate , &laser_node::callback, this);

        this->pub=nh.advertise<geometry_msgs::Twist>("/cmd_vel", rate);
        //  std::cout<<"Message from 1 ray: "<<msg.ranges[20];
    }

    void callback(const sensor_msgs::LaserScan::ConstPtr &msg)
    {
         double dist_right=msg->ranges[0];
         double dist_left=msg->ranges[719];
         double error=dist_left-dist_right;
         double p_error,d_error,old_error=0.0;
         double effort;
         // ---- Proportional logic-------//
         p_error=kp*error;
         d_error=(error-old_error)/1.0; // can sample by rate as well

         //---- Effort ----- //
         effort=kp*p_error+kd*d_error;
         ROS_INFO("effort: %f error: %f ", effort,error);
        //  move.linear.x=-2.0;  //base speed 
         
         
             move.linear.x=double(-5.0);
             move.angular.z=double(4.0);  //angular heading correction
             pub.publish(move);
             usleep(10);
         
        

    }
};

void laser_node::loop_function()
{
    move.linear.x=-5.0;
    pub.publish(move);
    usleep(10);
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "laser_node");
    laser_node custom_node(100);

    ros::spin();

    exit(0);
}

/*
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
*/
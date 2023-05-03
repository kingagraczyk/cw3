#include "ros/ros.h"
#include "sine_pkg/my_msg.h"
#include "std_msgs/Float32.h"
#include "boost/thread.hpp"
#include <iostream>

using namespace std;

class ROS_SUB {
	public:
		ROS_SUB();
		void topic_cb( sine_pkg::my_msgConstPtr data);
        void low_filter_thread();
	
	private:
		ros::NodeHandle _nh;
		ros::Subscriber _topic_sub;
        ros::Publisher _topic_pub;
        float f_value;
        float f_frequency; 
};

ROS_SUB::ROS_SUB() {
	
	_topic_sub = _nh.subscribe("/sine", 1, &ROS_SUB::topic_cb, this);
    boost::thread filter_thread( &ROS_SUB::low_filter_thread, this);
}

void ROS_SUB::topic_cb( sine_pkg::my_msgConstPtr d) {

	ROS_INFO("Listener value: %f", d->value);
    ROS_INFO("Listener amplitude: %f", d->amplitude);
    ROS_INFO("Listener frequency: %f", d->frequency);
    f_value = d->value;
    f_frequency = d->frequency;

}

void ROS_SUB::low_filter_thread(){
    _topic_pub = _nh.advertise<std_msgs::Float32>("/sine_filtered", 1);
    ros::Rate rate(10);
    float cut_freq = 50;
    while(ros::ok()){
        std_msgs::Float32 msg; 
        float gain = 1-(f_frequency - cut_freq)/100;
        if(f_frequency >= 100+cut_freq) gain = 0;
        if(f_frequency <= cut_freq) gain = 1;
        cout<<"[ INFO] [1679676594.404039906]: GAIN: "<<gain<<endl; 
        msg.data = gain*f_value;
        cout<<endl<<endl;
        ROS_INFO("Publish filtered sine value: %f",msg.data);
        _topic_pub.publish(msg); 
        rate.sleep();
    }
}

int main( int argc, char** argv ) {

	ros::init(argc, argv, "subscriber");

	ROS_SUB rs;
	
	ros::spin(); 

	return 0;
}

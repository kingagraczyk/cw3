#include "ros/ros.h" 
#include "sine_pkg/my_msg.h"
#include <iostream>
#include "math.h"
#define pi 3.14159265
 
using namespace std;


int main(int argc, char **argv) {

	ros::init(argc, argv,"sine_publisher");
	
	ros::NodeHandle nh;

	//Createing a publisher object:
	ros::Publisher topic_pub = nh.advertise<sine_pkg::my_msg>("/sine", 1);

	//Rate object: 10 Hz of rate
	ros::Rate rate(10); 

    //Defining the custom data type
    sine_pkg::my_msg sine;

    cout<<"insert the amplitude \n";
    cin>>sine.amplitude;
    cout<<"insert the frequency in Hz\n";
    cin>>sine.frequency;

    float Ts = 0.0001;

    int i = 0;

	while ( ros::ok() ) {

        sine.value = sine.amplitude*sin(2*pi*sine.frequency*i*Ts);
        i++;

		ROS_INFO("%f",sine.value); 

		topic_pub.publish(sine);
		
		//Rate to maintain the 10 Hz
		rate.sleep();
	}
	
	return 0;
}

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sstream> // for converting the command line parameter to integer

int main(int argc, char** argv)
{
  cv::VideoCapture vcap;
  ros::init(argc, argv, "image_publisher");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  image_transport::Publisher pub = it.advertise("ipcamstream/image", 1);

  std::string videoStreamString;
  nh.param<std::string>("ipcamstream/string", videoStreamString, "http://127.0.0.1:8080/?action=stream"); 
  std::cout << "Trying to open " << videoStreamString << std::endl;
  if(!vcap.open(videoStreamString)) {
	std::cout << "Error opening video stream or file" << std::endl;
	ros::shutdown();
  }

  cv::Mat frame;
  sensor_msgs::ImagePtr msg;

  ros::Rate loop_rate(5);
  while (nh.ok()) {
    vcap >> frame;
    // Check if grabbed frame is actually full with some content
    if(!frame.empty()) {
      msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
      pub.publish(msg);
      cv::waitKey(1);
    }

//     ros::spinOnce();
//     loop_rate.sleep();
  }
}
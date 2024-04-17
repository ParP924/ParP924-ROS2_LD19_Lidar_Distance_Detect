/*
- This file contains functions pertaining to distance from the lidar sensor
- Distance measurement calculated from lidar scans
*/

/*
INCLUDES
*/

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_msgs/msg/float32.hpp"

/*
MACROS
*/

#define LD_SCAN_SIZE 453
#define FWD_INDEX (LD_SCAN_SIZE / 2)
#define METERS_FT 3.28084
#define SAFE_DISTANCE_UPPERBOUND 8.0
#define SAFE_DISTANCE_LOWERBOUND 3.0


/*
Function: follow_object
    - Follows an object in front at a safe distance
    - Returns: void
    - Param: fwd_distance float
*/
void follow_object(float fwd_distance)
{
    // convert meters to feet
    float distance_feet = fwd_distance * METERS_FT;
    //printf("Distance in feet: %f\n", distance_feet);

}

/*
Function: detect_distance
    - Detects distance using scan message from LIDAR sensor
    - Returns: double value representing distance
    - Param: scan message from LIDAR scan
    - Run ros2 interface show sensor_msgs/msg/LaserScan for information about message
*/
float detect_distance(sensor_msgs::msg::LaserScan::SharedPtr scan)
{

    float forward_angle_distance = scan->ranges[FWD_INDEX];
    //printf("Distance to front: '%f\n", forward_angle_distance);

    return forward_angle_distance;
}

/*
Function: publish_distance
    - publishes fwd_distance as a topic
    - Returns: void
    - Param: forward_distance
*/
void publish_distance(const rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr& publisher, float fwd_distance)
{
    // Publishes fwd_distance as a topic
    auto message = std::make_shared<std_msgs::msg::Float32>();
    message->data = fwd_distance;
    publisher->publish(*message);

}

class ScanSubscriber : public rclcpp::Node
{
    public:
        ScanSubscriber() : Node("scan_subscriber")
        {
            publisher_ = this->create_publisher<std_msgs::msg::Float32>("forward_distance", 10);

            auto topic_callback = [this](sensor_msgs::msg::LaserScan::SharedPtr msg) -> void {
                //RCLCPP_INFO(this->get_logger(), "Message: '%f'", msg->ranges[i]);
                //i++;
                float fwd_distance = detect_distance(msg);
                publish_distance(publisher_, fwd_distance);
                //follow_object(fwd_distance);
            };

            subscriber_ = this->create_subscription<sensor_msgs::msg::LaserScan>("scan", 10, topic_callback);
        }

    private:
        rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr subscriber_;
        rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr publisher_;
        int i = 0;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto scan_subscriber = std::make_shared<ScanSubscriber>();
  rclcpp::spin(scan_subscriber);
  rclcpp::shutdown();
  return 0;
}
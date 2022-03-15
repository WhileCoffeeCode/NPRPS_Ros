#include "rclcpp/rclcpp.hpp"
#include <iostream>
#include "geometry_msgs/msg/pose_stamped.hpp"
#include <wiringPi.h>

using std::placeholders::_1;

#define MOTOR_PIN 12

class Motion : public rclcpp::Node {

    public:
        Motion():Node("Motion"){
            subscription_ = this->create_subscription<geometry_msgs::msg::PoseStamped>(
                pub_name, 10, std::bind(&Motion::topic_callback, this, _1)
            );

            if(subscription_ != nullptr){
                motion_move();
            }

        }

    private:
        void topic_callback(geometry_msgs::msg::PoseStamped message){
            real_PoseStampd = message;
            RCLCPP_INFO(this->get_logger(), "X: %d , Y: %d, Z: %d \n", 
                    real_PoseStampd.pose.position.x,
                    real_PoseStampd.pose.position.y,
                    real_PoseStampd.pose.position.z);
                                
        }//End of topic_callback

        /**
         * @brief In this function we are trying to move the robot to forward through GPIO pins
         */
        void motion_move(){
            for(;;){
                
                if(real_PoseStampd.pose.position.x < 3.0){
                    //Move Forward
                    //Give Signal to Move Forward
                    digitalWrite(MOTOR_PIN, 255);
                    
                
                }else{
                    //Stop Moving
                    //give signal to stop moving
                    digitalWrite(MOTOR_PIN, 0);
                }
            }//End of forever loop
        }//End of Motion Move 
        

    char * pub_name = "/RigidBody3/pose";
    geometry_msgs::msg::PoseStamped real_PoseStampd;     
    rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr subscription_;            
};


int main(int argc, char *argv[]){
    rclcpp::init(argc, argv);
    wiringPiSetupGpio();
    pinMode(MOTOR_PIN, OUTPUT);

    rclcpp::spin(std::make_shared<Motion>());

    rclcpp::shutdown();
    return 0;
}

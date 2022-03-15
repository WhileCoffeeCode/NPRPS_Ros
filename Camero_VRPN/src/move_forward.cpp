#include "rclcpp/rclcpp.hpp"
#include <iostream>
#include "geometry_msgs/msg/pose_stamped.hpp"
#include <unistd.h>
#include <cmath>

using std::placeholders::_1;



class Motion : public rclcpp::Node {

    public:
        Motion():Node("Motion"){
            subscription_ = this->create_subscription<geometry_msgs::msg::TwistStamped>(
                "/RigidBody1/pose", 15, std::bind(&Motion::topic_callback, this, _1)
            );

            motion_move();

        }

    private:
        void topic_callback(geometry_msgs::msg::TwistStamped message){
            real_PoseStampd = message;
            float velocity_value = velocityCalculator(real_PoseStampd.twist.linear.x, real_PoseStampd.twist.linear.y);
            RCLCPP_INFO(this->get_logger(), "X: %f , Y: %f, Z: %f \n", real_PoseStampd.pose.position.x,real_PoseStampd.pose.position.y,real_PoseStampd.pose.position.z);
            RCLCPP_INFO(this->get_logger(),"Velocity is %f \n", );

            //We are going to call the Speed Controller each time we get the data

            // motion_move();                    
        }//End of topic_callback

        /**
         * @brief In this function we are trying to move the robot to forward through GPIO pins
         */
        void motion_move(){
                
                if(real_PoseStampd.pose.position.x < 3.0){
		            //Give Signal to Move Forward
                    if(move_flag){
                        //Move Forward
                        fprintf(stderr, "moving  Forward %d \n", move_flag); 
                        system(command_move);
                        move_flag = false;
                        stop_flag = true;
                    }  
                }else{
                     //give signal to stop moving
                        if(stop_flag){
                            //Stop Moving
			                fprintf(stderr, "Stoping the motor \n");
                            system(command_stop);
                            stop_flag = false;
                            move_flag = true;
                    }
                    
                }
		    usleep(250000);
        }//End of Motion Move 
        
        /**
        * @brief This function gets the Vx and Vy and returns Velocity
        * @return float velocity 
        */
        float velocityCalculator(float vx, float vy){
            return sqrt(pow(vx,2)+ pow(vy,2));
        }//End of velocityCalculator method

        

        //TODO: At the moment the desired speed is hardcoded, i have to change this later
        //TODO: Gains are okay ?
        float speedController(float desired_speed, float measured_speed){
            float change_in_PWM = 0.0;
            //FeedForward, gain 0.05
            float FeedForward_for_desired = desired_speed * 0.1;
            
            //Feedback - gain 0.3
            float Feedback_for_measured = (desired_speed - measured_speed)*0.3;
            
            change_in_PWM = Feedback_for_measured + FeedForward_for_desired;

            return change_in_PWM;

        }//end of Speed Controller 
    
    
    
    
    geometry_msgs::msg::TwistStamped real_PoseStampd;     
    rclcpp::Subscription<geometry_msgs::msg::TwistStamped>::SharedPtr subscription_;      
    const char * command_move = "python3 /home/ubuntu/NPRPS_Ros/Camero_VRPN/src/led.py 1";
    bool move_flag = true;

    
    const char * command_stop = "python3 /home/ubuntu/NPRPS_Ros/Camero_VRPN/src/led.py 0";
    bool stop_flag = true;

};


int main(int argc, char *argv[]){
    rclcpp::init(argc, argv);
 

    rclcpp::spin(std::make_shared<Motion>());

    rclcpp::shutdown();
    return 0;
}

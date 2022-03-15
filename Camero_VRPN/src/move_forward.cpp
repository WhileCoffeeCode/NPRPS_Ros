#include "rclcpp/rclcpp.hpp"
#include <iostream>
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"
#include <unistd.h>
#include <cmath>
#include <string>
#include <cstdio>

using std::placeholders::_1;

using namespace std;

class Motion : public rclcpp::Node
{

public:
    Motion() : Node("Motion")
    {
        // The subscriber for Twist
        subscription_twist = this->create_subscription<geometry_msgs::msg::TwistStamped>(
            "/RigidBody1/twist",
            15,
            std::bind(&Motion::topic_callback_twist, this, _1));

        subscription_pose = this->create_subscription<geometry_msgs::msg::PoseStamped>(
            "/RigidBody1/pose",
            15,
            std::bind(&Motion::topic_callback_pose, this, _1));

    }

private:

    void topic_callback_pose(geometry_msgs::msg::PoseStamped message){
        ego_postion[0] =message.pose.position.x;
        ego_postion[1] = message.pose.position.z;
        if (ego_postion[0] >= 3.0){
            ARM_CONTROLLER = false;
            setSpeedandExecute(7.5);
        }
    }


    void topic_callback_twist(geometry_msgs::msg::TwistStamped message)
    {
        real_twist = message;
        // Extract the Vx, Vz
        float vx = real_twist.twist.linear.x;
        float vy = real_twist.twist.linear.z; // This is our Vy

        float velocity_value = velocityCalculator(vx, vy);
        RCLCPP_INFO(this->get_logger(), "[%f, %f, %f]\n",ego_postion[0], ego_postion[1], velocity_value);
        
        if(ARM_CONTROLLER){

            float speedController_ret = speedController(desired_velocity, velocity_value);
            float new_pwm = pwm + speedController_ret;
            RCLCPP_INFO(this->get_logger(), "PWM: %f \n", new_pwm);
            setSpeedandExecute(new_pwm);

        }

    } // End of topic_callback

    /**
     * @brief This function gets the Vx and Vy and returns Velocity
     * @return float velocity
     */
    float velocityCalculator(float vx, float vy)
    {
        return sqrt(pow(vx, 2) + pow(vy, 2));
    } // End of velocityCalculator method

    float speedController(float desired_speed, float measured_speed)
    {

        float change_in_PWM = 0.0;
        // FeedForward
        float FeedForward_for_desired = desired_speed * 0.2;
        // Feedback
        float Feedback_for_measured = (desired_speed - measured_speed) * 0.3;
        change_in_PWM = Feedback_for_measured + FeedForward_for_desired;
        change_in_PWM = change_in_PWM * 0.1;
        return change_in_PWM;

    }// end of Speed Controller

    void setSpeedandExecute(float givenSpeed)
    {
        float adjustedSpeed = 0.0;
        // we have to limit the maximum PWM which is going to be sent to the ESC
        // would be 8.7

        if (givenSpeed >= 8.7)
            adjustedSpeed = 8.7;
        else
            adjustedSpeed = givenSpeed;

        string new_command_move = command_move + to_string(adjustedSpeed);
        RCLCPP_INFO(this->get_logger(), "Sending PWM:%f   %s \n ", adjustedSpeed, new_command_move.c_str());
        system(new_command_move.c_str());
    } // End of SetSpeedandExecute



    geometry_msgs::msg::TwistStamped real_twist;
    float ego_postion[2]; 

    rclcpp::Subscription<geometry_msgs::msg::TwistStamped>::SharedPtr subscription_twist;
    rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr subscription_pose;

    string command_move = "python3 /home/ubuntu/NPRPS_Ros/Camero_VRPN/src/led.py 1 ";
    
    float desired_velocity = 0.5;
    float pwm = 8.2;

    bool ARM_CONTROLLER = false;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<Motion>());

    rclcpp::shutdown();
    return 0;
}

/*
       *Abyss

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
*/
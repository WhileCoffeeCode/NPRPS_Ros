#include <iostream>
#include <cstdio>
#include <cmath>
#include <thread>
#include <unistd.h>
#include <string>

using namespace std;

string command_move = "python3 /home/ubuntu/NPRPS_Ros/Camero_VRPN/src/led.py 1 ";

void setSpeedandExecute(float givenSpeed){
    float adjustedSpeed = 0.0;
    //we have to limit the maximum PWM which is going to be sent to the ESC
    //would be 8.7
    if(givenSpeed >= 8.7)
        adjustedSpeed = 8.7;
    else 
        adjustedSpeed = givenSpeed;
        


    string new_command_move = command_move + to_string(adjustedSpeed); 
    printf("Sending PWM:%f   %s \n ",adjustedSpeed,  new_command_move.c_str());
    system(new_command_move.c_str());

}


float velocityCalculator(float vx, float vy){
    return sqrt(pow(vx,2)+ pow(vy,2));
}//End of velocityCalculator method



//TODO: At the moment the desired speed is hardcoded, i have to change this later
float speedController(float desired_speed, float measured_speed){
    float change_in_PWM = 0.0;
    //FeedForward
    float FeedForward_for_desired = desired_speed * 0.2;
            
    //Feedback
    float Feedback_for_measured = (desired_speed - measured_speed)*0.3;
            
    change_in_PWM = Feedback_for_measured + FeedForward_for_desired;
    change_in_PWM = change_in_PWM * 0.1;

    return change_in_PWM;

}//end of Speed Controller 




void thread_function(){
    // bool pwm_did_not_change = true;

    float desired = 1.0;
    float measured = 0.0;
    //TODO: Check this value , when the battery full charge
    float pwm = 8.2;
    float speedController_ret = 0.0;

    for(int i =0; i < 3; i++){
        speedController_ret =  speedController(desired, measured);

        printf("SpeedController = %f \t measured = %f \n", 
            speedController_ret,
            measured);
        if (i == 0)
            measured = 0.2;
        else  
            measured = measured + 0.1;  
        
        if(measured >= 1.0)
            measured = 1.0;

        float new_pwm = speedController_ret + pwm;
        printf("PWM %f\n", new_pwm);
        
        printf("\nSending \n");
        setSpeedandExecute(new_pwm);
        sleep(1);
    }
    printf("verify %f \n", velocityCalculator(0.492, 0.49));
}//end of thread_function


int main(int argc, char *argv[]){
  
    printf("Calling the thread \n");
    
    //Goes to Neutral Mode/Activate Mode
    setSpeedandExecute(7.5);
    sleep(2);
    printf("Starting the Process\n");
    thread th1(thread_function);
    th1.join();
    
    setSpeedandExecute(7.5);



    return 0;
}//end of main method
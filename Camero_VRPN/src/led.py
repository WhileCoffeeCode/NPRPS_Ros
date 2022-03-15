import RPi.GPIO as GPIO
import time
import sys

#Make a daemon and make sure its reads value

value = int(sys.argv[1])
user_pwm_value = float(sys.argv[2])
print(user_pwm_value)
print(sys.argv[1]) 
if value > 1 or value < 0:
	print("[BLDC] Wrong PWM!")
	sys.exit(-1)

GPIO.setmode(GPIO.BCM)

GPIO.setwarnings(False)
GPIO.setup(17,GPIO.OUT)
t1 = GPIO.PWM(17, 50)
t1.start(2.5)

#Motor Run
if value == 1:
	print("[BLDC]Motor On!")
	t1.ChangeDutyCycle(user_pwm_value)
	time.sleep(1)
	# t1.ChangeDutyCycle(user_pwm_value)
	# time.sleep(1)
	print("After")
	t1.ChangeDutyCycle(0.0)
	time.sleep(0.1)


elif value == 0:
	print("[BLDC]Motor Off !")	
	t1.ChangeDutyCycle(7.5)
	time.sleep(1)
	t1.ChangeDutyCycle(0.0)
	time.sleep(0.1)





#Clean it up
print("Done")
t1.stop()
GPIO.cleanup()



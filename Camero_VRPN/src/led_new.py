import RPi.GPIO as GPIO
import time
import sys

value = int(sys.argv[1])
print(sys.argv[1]) 
if value > 1 or value < 0:
	print("[BLDC] Wrong PWM!")
	sys.exit(-1)

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(13,GPIO.OUT)
t1 = GPIO.PWM(13, 50)
time.sleep(1)
t1.start(7.5)
time.sleep(10);

#Motor Run
if value == 1:
	print("[BLDC]Motor On!")
	t1.ChangeDutyCycle(7.50)
	time.sleep(20)

elif value == 0:
	print("[BLDC]Motor Off !")	
	t1.ChangeDutyCycle(7.0)
	time.sleep(1)





#Clean it up
t1.stop()


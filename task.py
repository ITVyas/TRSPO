import time
import threading 
from datetime import datetime


def EasyFunction():
	time.sleep(2)

def MediumFunction():
	time.sleep(3)

def HardFunction():
	time.sleep(5)

startTime = datetime.now()

Thread1 = threading.Thread(target = EasyFunction, args = ())
Thread2 = threading.Thread(target = MediumFunction, args = ())
Thread3 = threading.Thread(target = HardFunction, args = ())

Thread1.start()
Thread2.start()
Thread3.start()

Thread1.join()
Thread2.join()
Thread3.join()

endTime = datetime.now()

print(endTime-startTime)


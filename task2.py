import threading 
import random as rand
from datetime import datetime
from multiprocessing import Pool


class Shirt:
	def __init__(self):
		self.priceDollars = 10
		pass

	def __init__(self,price):
		self.priceDollars = price 
		pass

	def getPrice(self):
		return self.priceDollars

	def setPrice(self, price):
		self.priceDollars = price
		pass 

class Pants:
	def __init__(self):
		self.priceDollars = 10
		pass

	def __init__(self,price):
		self.priceDollars = price 
		pass

	def getPrice(self):
		return self.priceDollars

	def setPrice(self, price):
		self.priceDollars = price
		pass 



lock1 = threading.Lock()
lock2 = threading.Lock()

def addRandFirstPart(shirt, pants, times = 1):
	lock1.acquire()
	try:
		for i in range(times):
			shirt.setPrice(shirt.getPrice() + rand.randrange(1, 500)/10)
	finally:
		lock1.release()

	lock2.acquire()
	try:
		for i in range(times):
			pants.setPrice(pants.getPrice() + rand.randrange(1, 500)/10)
	finally:
		lock2.release()
	pass

def addRandSecondPart(shirt, pants, times = 1):
	lock2.acquire()
	try:
		for i in range(times):
			pants.setPrice(pants.getPrice() + rand.randrange(1, 500)/10)
	finally:
		lock2.release()

	lock1.acquire()
	try:
		for i in range(times):
			shirt.setPrice(shirt.getPrice() + rand.randrange(1, 500)/10)
	finally:
		lock1.release()
	pass

def main():
	start = datetime.now()
	shirt, pants = Shirt(15.5), Pants(16.2)

	N = rand.randrange(10,21,2) 
	K1, K2 = rand.randrange(10000,20001), rand.randrange(10000, 20001)

	threads = []
	for i in range(round(N/2)):
		threads.append(threading.Thread(target = addRandFirstPart, args = (shirt, pants, K1)))
		threads[i].start()

	for i in range(round(N/2)):
		threads.append(threading.Thread(target = addRandSecondPart, args = (shirt, pants, K2)))
		threads[round(N/2)+i].start()


	for i in range(N):
		threads[i].join()

	print(f"Done. Time is {datetime.now()-start}")
	pass

main()



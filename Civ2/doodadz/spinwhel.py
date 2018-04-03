import random
from PIL import Image

#function to print out 2d list line by line
def print2D(list2d):
	for row in list2d:
		print row

def spindleGen():
	im = Image.open('Civ2/Civ2/doodadz/spinwhel32_seco_bare.png')
	pixelMap = im.load()

	#Gonna make a spindle from
	#(5,7) and below until  (5,19)

	#long spikey boy first
	for i in range(0,3):
		pixelMap[5,7+i] = (255,255,255,255)

	#start making the wobbly wankii
	radius = random.randint(1,2)

	#we need to now start filling in (5,10 and below)
	for i in range(0,10): #for each line
		#the span of the radius
		for j in range(0,radius+1): #for each point away from center along radius
			pixelMap[5+j, 10+i] = (255,255,255,255)
			pixelMap[5-j, 10+i] = (255,255,255,255)

		#Now generate a new radius
		radius = radius + random.randint(-2,2)

		#radius bounds checking...
		if radius < 0:
		 	radius = 0
		if radius > 3:
		 	radius = 3

		 #End for loop
		 #Will now move on to next line





	im.show()
	im.save("Civ2/Civ2/doodadz/spinwhelSeco.png","PNG")

	


spindleGen()
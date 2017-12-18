#Generate a screw press
#A basic frame with a randmoly generated screw :))))
#OUtputs a 32x32 PNG

#First generate screw
#Then plop the screw on a 32x32 
#Then exit out a band 

import random

#GLOBALS
size = 16

#16 bits
def generateScrew():
	screw = [] #a 2D list for the pattern

	#initialize list
	for i in range(0, size):
		row = [] #individual row
		for j in range(0, size):
			row.append(' ')
		screw.append(row)

	#We're going to create a screw from the head, to the stalk, to the thread
	#first create the SCREW HEAD
	radius = random.randint(5,7) #radius of the head
	for i in range(0,radius):
		screw[13][7-i] = "*"
		screw[13][8+i] = "*"
	
	#Decide if we want wings on the screw head
	if(random.randint(0,1) == 1):
		screw[12][7-i] = "*"
		screw[12][8+i] = "*"

	#Decide if we want another layer on screw head
	if(random.randint(0,1) == 1):
		radius = random.randint(2,radius)
		for i in range(0,radius):
			screw[14][7-i] = "*"
			screw[14][8+i] = "*"

	#next, we create the STALK
	length = random.randint(10,12)
	thickness = random.randint(1,3)
	for i in range(0,length):
		for j in range(0,thickness):
			screw[12-i][7-j] = "*"
			screw[12-i][8+j] = "*"

	#finally, we create the THREAD
	thread = random.randint(1,3) #how long the threads are on the sides
	g = 13-length #keeps track of where we need to add threading next (start at bottom)
	#h = 0 #a counter, if we 
	while(True):

		if((g/thread)%2==0):
			screw[g][8+thickness]="*"
		else:
			screw[g][7-thickness]="*"

		g = g+1
		if(g == 13):
			break

	#We have to blot out the area we don't want (where prim is)
	for i in range(0,16):
		screw[9][i] = ""
	screw[10][8] = ""
	screw[8][7] = ""
	screw[10][6] = ""
	screw[8][5] = ""
	screw[10][4] = ""

	return screw



######
#NOW FOR PIL IMAGE CREATION
from PIL import Image

#writes a new png image, given a filename and a weed 2d array list
def screwWrite(filename, screw_in):
	im = Image.open('Civ2/Civ2/doodadz/blank32.png')
	#im = Image.open('test.png')
	pixelMap = im.load()

	img = Image.new( im.mode, im.size)
	pixelsNew = img.load()
	#for i in range(img.size[0]-19, img.size[0]-13):
	for i in range(13,29):
	    #for j in range(img.size[1]-24):
	    for j in range(8,24):
	    	if screw_in[j-8][i-13] == '*':
	        	pixelsNew[i,j] = (255,255,255,255)

	#img.show()
	img.save(filename,"PNG")


temp_screw = generateScrew()
#print temp_screw
screwWrite("Civ2/Civ2/doodadz/screwpressSeco.png",temp_screw)







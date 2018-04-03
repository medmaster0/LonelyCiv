#generates random 16 bit patterns of stonez
#smooth gentle formation
#I'm DRUNK when i wrote this!
import random

#GLOBALS
size = 16

#function to print out 2d list line by line
def print2D(list2d):
	for row in list2d:
		print row

#16 bits
def generateStone():
	stone = [] #a 2D list for the pattern

	#initialize list
	for i in range(0, size):
		row = [] #individual row
		for j in range(0, size):
			row.append(' ')
		stone.append(row)

	#we're gonna create a smooth stone, from bottom up, line by line
	#lines are symmetrical!, centered
	#to achieve smoothing effect, each line is either 1 greater or 1 less (or same) as prev 
	radius = random.randint(2,4) #radius is HALF the size of the line

	for i in range(0,random.randint(14,15)): #will cycle through all rows except top one or two
		for j in range(0,radius): #fill out a line between the two limits
			try:
				stone[15-i][7-j]='*'
				stone[15-i][8+j]='*'
			except:
				z = z+1
				#print "ignoring bounds error"
		#Generate new radius, partly based on current radius
		#maybe a little bigga, maybe a little smalla
		radius = radius + random.randint(-2,+1)
		# if radius < 0:
		#  	radius = 0
		# if radius == 7:
		#  	radius = 6
		#New radius to leave room for outline
		if radius < 1:
		 	radius = 1
		if radius == 6:
		 	radius = 5

	return stone


######
#NOW FOR PIL IMAGE CREATION
from PIL import Image

#writes a new png image, given a filename and a weed 2d array list
def stoneWrite(filename, stone_in):
	im = Image.open('Civ2/Civ2/weedz/blank.png')
	#im = Image.open('test.png')
	pixelMap = im.load()

	img = Image.new( im.mode, im.size)
	pixelsNew = img.load()
	for i in range(img.size[0]):
	    for j in range(img.size[1]):
	    	if stone_in[j][i] == '*':
	        	pixelsNew[i,j] = (255,255,255,255)

	#img.show()
	img.save(filename,"PNG")

temp_stone = generateStone()
#print temp_stone

#Actual program
#These filenames might be a little wonky
#They are from the top level of the Cplus folder. 
#It's where from the context of the C++ program calling this...
for z in range(100,200):
	temp_stone = generateStone()
	stoneWrite("Civ2/Civ2/stonez/"+str(z)+".png",temp_stone)


#generates random 16 bit patterns of fruitz
#bulbous formation
#I'm DEPRESSEDDDDDD when i wrote this!
import random
from PIL import Image

#GLOBALS
size = 16

#function to print out 2d list line by line
def print2D(list2d):
	for row in list2d:
		print row

#16 bits
def generateFruit():
	fruit = [] #a 2D list for the pattern

	#random offset (just to add variations)
	offset = random.randint(-1,1)

	#initialize list
	for i in range(0, size):
		row = [] #individual row
		for j in range(0, size):
			row.append(' ')
		fruit.append(row)

	#now go row by row hitting up all dem rows, updating swath
	radius = random.randint(1,2); 
	for i in range(2,random.randint(7,9)): #will cycle through random amount of rows starting at 2
		for j in range(0,radius): #fill out a pixel for the radius
			try:
				fruit[i][4-j+offset]='*'
				fruit[i][5+j+offset]='*'
			except:
				print "ignoring bounds error"
		#Generate new random radius, partly based on current radius 
		#maybe a little bigga, maybe a little smalla
		if(random.randint(1,2)==1):
			radius = radius - 1
		else:
			radius = radius + 1

		#bounds checking
		if(radius==0):
			radius = 1
		if(radius==4):
			radius = 3

	#print2D( weed )
	return fruit

#writes a new png image, given a filename and a weed 2d array list
def fruitWrite(filename, fruit_in):
	im = Image.open('Civ2/Civ2/fruitz/blank.png')
	#im = Image.open('test.png')
	pixelMap = im.load()

	img = Image.new( im.mode, im.size)
	pixelsNew = img.load()
	for i in range(img.size[0]):
	    for j in range(img.size[1]):
	    	if fruit_in[j][i] == '*':
	        	pixelsNew[i,j] = (255,255,255,255)

	#img.show()
	img.save(filename,"PNG")


#Actual program
#These filenames might be a little wonky
#They are from the top level of the Cplus folder. 
#It's where from the context of the C++ program calling this...
for z in range(200,300):
	temp_fruit = generateFruit()
	fruitWrite("Civ2/Civ2/fruitz/"+str(z)+".png",temp_fruit)






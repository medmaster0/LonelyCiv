#generates random 16 bit patterns of weeds
#plant-like curl
#I'm DRUNK when i wrote this!
import random

#GLOBALS
size = 16

#function to print out 2d list line by line
def print2D(list2d):
	for row in list2d:
		print row

#16 bits
def generateWeed():
	weed = [] #a 2D list for the pattern
	z = 0

	#initialize list
	for i in range(0, size):
		row = [] #individual row
		for j in range(0, size):
			row.append(' ')
		weed.append(row)

	#Start construction, bottom up
	# swath = random.randint(8,12) #a "line of weed", 
	# #Always start at the same point (bottom left, 1 pixel right from corner): (15,1)
	# weed[15][1]= 1
	# for i in range(0,swath):
	# 	weed[15][1+i]='*'

	#now go row by row hitting up all dem rows, updating swath
	# baseL = 8 #lower limit of swath
	# baseH = 12 #upper limit of swath
	baseL = random.randint(1,8)
	baseH = random.randint(baseL, 12)
	#swath = random.randint(baseL, baseH)
	for i in range(0,random.randint(14,15)): #will cycle through all rows except top one or two
		for j in range(baseL,1+baseH): #fill out a line between the two limits
			try:
				if (j+1)<0:
					j = -1
				weed[15-i][1+j]='*'
			except:
				#print "ignoring bounds error"
				z = z+1
		#Generate new random swath, partly based on current swath 
		#maybe a little bigga, maybe a little smalla
		baseL = random.randint(baseL-1,baseH)
		baseH = random.randint(baseL,baseH+1)
		#baseH = swath+1
		#swath = random.randint(baseL,baseH)
		#print swath
		#print (baseL, baseH)

	#print2D( weed )
	return weed


# random_weed = generateWeed()
# print2D(random_weed)

######
#NOW FOR PIL IMAGE CREATION
from PIL import Image

#writes a new png image, given a filename and a weed 2d array list
def weedWrite(filename, weed_in):
	im = Image.open('Civ2/Civ2/weedz/blank.png')
	#im = Image.open('test.png')
	pixelMap = im.load()

	img = Image.new( im.mode, im.size)
	pixelsNew = img.load()
	for i in range(img.size[0]):
	    for j in range(img.size[1]):
	    	if weed_in[j][i] == '*':
	        	pixelsNew[i,j] = (255,255,255,255)

	#img.show()
	img.save(filename,"PNG")

#Actual program
#These filenames might be a little wonky
#They are from the top level of the Cplus folder. 
#It's where from the context of the C++ program calling this...
for z in range(0,100):
	temp_weed = generateWeed()
	weedWrite("Civ2/Civ2/weedz/"+str(z)+".png",temp_weed)


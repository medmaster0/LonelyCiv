#SCRIPT  to outline png images
#should do some primitive edge detection 
#Finds transparent-white edge
#changes transparent pixel black

######
#NOW FOR PIL IMAGE CREATION
from PIL import Image

def outlineFile(filename):
	#OPen for editting
	im = Image.open(filename)
	pixelMap = im.load()

	#Go line by line, pixel by pixel
	for i in range(0,16): #spans Y
		for j in range(0,16): #spans X

			#Change transparent on top of white to black 
			if i <15: #bounds checking
				if pixelMap[j,i][3]==0: #if transparent
					if pixelMap[j,i+1][3]==255 and pixelMap[j,i+1][2]==255:#Now check if the one below is white
						pixelMap[j,i] = (0,0,0,255)

			#Change transparent on bottom of white to black
			if i > 0: #bounds checking
				if pixelMap[j,i][3]==0: #if transparent
					if pixelMap[j,i-1][3]==255 and pixelMap[j,i-1][2]==255: #Now check if the one above is white
						pixelMap[j,i] = (0,0,0,255)

			#Change transparent on left of white to black
			if j < 15: #bounds checking
				if pixelMap[j,i][3]==0: #if transparent
					if pixelMap[j+1,i][3]==255 and pixelMap[j+1,i][2]==255: #Now check if the one right is white
						pixelMap[j,i] = (0,0,0,255)

			#Change transparent on right of white to black
			if j > 0: #bounds checking
				if pixelMap[j,i][3]==0: #if transparent
					if pixelMap[j-1,i][3]==255 and pixelMap[j-1,i][2]==255: #Now check if the one left is white
						pixelMap[j,i] = (0,0,0,255)

	#Now we gotta go over again and check CORNERS!!!!
	for i in range(0,16): #spans Y
		for j in range(0,16): #spans X
	 		if pixelMap[j,i][3]==0: #if transparent

	 			#TOP LEFT
	 			try:
		 			if pixelMap[j,i+1][3]==255 and pixelMap[j+1,i][3]==255:
		 				pixelMap[j,i] = (0,0,0,255)
		 		except:
		 			x = 0

		 		 #TOP Right
	 			try:
		 			if pixelMap[j,i+1][3]==255 and pixelMap[j-1,i][3]==255:
		 				pixelMap[j,i] = (0,0,0,255)
		 		except:
		 			x = 0

		 		#BOTTOM Right
	 			try:
		 			if pixelMap[j,i-1][3]==255 and pixelMap[j-1,i][3]==255 \
		 			and pixelMap[j-1,i-1][2]==255: #make sure adjacent corner piece is white
		 				pixelMap[j,i] = (0,0,0,255)
		 		except:
		 			x = 0

		 		#BOTTOM LEFT
	 			try:
		 			if pixelMap[j,i-1][3]==255 and pixelMap[j+1,i][3]==255 \
		 			and pixelMap[j+1,i-1][2]==255: #make sure adjacent corner piece is white
		 				pixelMap[j,i] = (0,0,0,255)
		 		except:
		 			x = 0


	#Overwrite Old file
	im.save(filename,"PNG")

for i in range(0,100):
	outlineFile("Civ2/Civ2/weedz/"+str(i)+".png")

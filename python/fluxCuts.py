import numpy as np

def getOVCBoundaries(x,y):

        #Recall from N-MISC-16-001 pg 33 the position of the source
	d = 8.1534 #cm
	d*=(1/100.0)
	xsrc = d*np.cos(60*(np.pi/180.0)) #60 deg
	ysrc = d*np.sin(60*(np.pi/180.0)) #60 deg

	#make the cuts as I go along
	cuts = {}

	#make a circle for primary location
	npts = 100
	rcirc = 0.1
	thet = np.arange(npts)*(2*np.pi/np.float(npts))
	xsrc = np.ones(np.shape(thet))*xsrc
	ysrc = np.ones(np.shape(thet))*ysrc
	xcirc = xsrc + rcirc*np.cos(thet)
	ycirc = ysrc + rcirc*np.sin(thet)


	r = 0.25

	#make a line for location B
	deg0B = 255*(np.pi/180.0) #do a 230 deg start line
	deg1B = 25*(np.pi/180.0) #do a 20 deg end line
	x0B = r*np.cos(deg0B)
	y0B = r*np.sin(deg0B)
	x1B = r*np.cos(deg1B)
	y1B = r*np.sin(deg1B)
	mB = (y1B-y0B)/(x1B-x0B)
	bB = y0B - mB*x0B
	xlineB = np.ones(np.shape(np.arange(npts)))*x0B + np.arange(npts)*(x1B-x0B)/(np.float(npts))
	ylineB = mB*xlineB + np.ones(np.shape(np.arange(npts)))*bB
	fullthetB = (360-255+25)*(np.pi/180.0) #do a 20 deg arc
	thetB = np.arange(npts)*(fullthetB/np.float(npts))
	thetB0 = 255*(np.pi/180.0) #starting at 50 deg
	thetB0 = np.ones(np.shape(thetB))*thetB0
	thetB = thetB0 + thetB
	xB = r*np.cos(thetB)
	yB = r*np.sin(thetB)
	xB = np.concatenate((xB,np.flip(xlineB,0)),0)
	yB = np.concatenate((yB,np.flip(ylineB,0)),0)



	#make a line for location D
	deg0D = 80*(np.pi/180.0) #do a 80 deg start line
	deg1D = 195*(np.pi/180.0) #do a 170 deg end line
	x0D = r*np.cos(deg0D)
	y0D = r*np.sin(deg0D)
	x1D = r*np.cos(deg1D)
	y1D = r*np.sin(deg1D)
	mD = (y1D-y0D)/(x1D-x0D)
	bD = y0D - mD*x0D
	xlineD = np.ones(np.shape(np.arange(npts)))*x0D + np.arange(npts)*(x1D-x0D)/(np.float(npts))
	ylineD = mD*xlineD + np.ones(np.shape(np.arange(npts)))*bD
	fullthetD = (deg1D-deg0D) #do a X deg arc
	thetD = np.arange(npts)*(fullthetD/np.float(npts))
	thetD0 = 80*(np.pi/180.0) #starting at 50 deg
	thetD0 = np.ones(np.shape(thetD))*thetD0
	thetD = thetD0 + thetD
	xD = r*np.cos(thetD)
	yD = r*np.sin(thetD)
	xD = np.concatenate((xD,np.flip(xlineD,0)),0)
	yD = np.concatenate((yD,np.flip(ylineD,0)),0)


	#make a semicircle for location A
	deg0A = 7*(np.pi/180.0) #do a 7 deg start line
	deg1A = 90*(np.pi/180.0) #do a 90 deg end line
	x0A = r*np.cos(deg0A)
	y0A = r*np.sin(deg0A)
	x1A = r*np.cos(deg1A)
	y1A = r*np.sin(deg1A)
	mA = (y1A-y0A)/(x1A-x0A)
	bA = y0A - mA*x0A
	xlineA = np.ones(np.shape(np.arange(npts)))*x0A + np.arange(npts)*(x1A-x0A)/(np.float(npts))
	ylineA = mA*xlineA + np.ones(np.shape(np.arange(npts)))*bA


	xAB = (bB-bA)/(mA-mB) #find cross-over point with B
	xAD = (bD-bA)/(mA-mD) #find cross-over point with D
	xlineAr = xlineA[(xlineA<xAB)&(xlineA>xAD)] #restrict to end at line of B and D
	ylineAr = ylineA[(xlineA<xAB)&(xlineA>xAD)]
	xlineA = xlineAr

	xlineArr = np.concatenate((xlineD[xlineD>xAD],np.flip(xlineAr,0),xlineB[xlineB>xAB]),0)
	ylineArr = np.concatenate((ylineD[xlineD>xAD],np.flip(ylineAr,0),ylineB[xlineB>xAB]),0)

	fullthetA = (deg0D-deg1B)
	thetA = np.arange(npts)*(fullthetA/np.float(npts))
	thetA0 = deg1B
	thetA0 = np.ones(np.shape(thetA))*thetA0
	thetA = thetA0 + thetA
	xA = r*np.cos(thetA)
	yA = r*np.sin(thetA)
	xlineAfull = np.concatenate((xA,xlineArr))
	ylineAfull = np.concatenate((yA,ylineArr))


	#make a semicircle for location C
	deg0C = 145*(np.pi/180.0) #do a 7 deg start line
	deg1C = 310*(np.pi/180.0) #do a 90 deg end line
	x0C = r*np.cos(deg0C)
	y0C = r*np.sin(deg0C)
	x1C = r*np.cos(deg1C)
	y1C = r*np.sin(deg1C)
	mC = (y1C-y0C)/(x1C-x0C)
	bC = y0C - mC*x0C
	xlineC = np.ones(np.shape(np.arange(npts)))*x0C + np.arange(npts)*(x1C-x0C)/(np.float(npts))
	ylineC = mC*xlineC + np.ones(np.shape(np.arange(npts)))*bC

	xCB = (bB-bC)/(mC-mB) #find cross-over point with B
	xCD = (bD-bC)/(mC-mD) #find cross-over point with D
	xlineCr = xlineC[(xlineC<xCB)&(xlineC>xCD)] #restrict to end at line of B and D
	ylineCr = ylineC[(xlineC<xCB)&(xlineC>xCD)]

	xlineCrr = np.concatenate((xlineB[xlineB<xCB],np.flip(xlineCr,0),xlineD[xlineD<xCD]),0)
	ylineCrr = np.concatenate((ylineB[xlineB<xCB],np.flip(ylineCr,0),ylineD[xlineD<xCD]),0)

	fullthetC = (deg0B-deg1D) #do a X deg arc
	thetC = np.arange(npts)*(fullthetC/np.float(npts))
	thetC0 = deg1D
	thetC0 = np.ones(np.shape(thetC))*thetC0
	thetC = thetC0 + thetC
	xC = r*np.cos(thetC)
	yC = r*np.sin(thetC)
	xlineCfull = np.concatenate((xC,xlineCrr))
	ylineCfull = np.concatenate((yC,ylineCrr))

	boundaries = {}
	boundaries['Pr'] = {}
	boundaries['A'] = {}
	boundaries['B'] = {}
	boundaries['C'] = {}
	boundaries['D'] = {}

	boundaries['Pr']['x'] = xcirc
	boundaries['Pr']['y'] = ycirc
	boundaries['A']['x'] = xlineAfull
	boundaries['A']['y'] = ylineAfull
	boundaries['B']['x'] = xB
	boundaries['B']['y'] = yB
	boundaries['C']['x'] = xlineCfull
	boundaries['C']['y'] = ylineCfull
	boundaries['D']['x'] = xD
	boundaries['D']['y'] = yD

	return boundaries

#!/bin/sh


#3/2/18
#do just the along bridge shift with updated code develop tag elog/445
# loop to submit many jobs 
./condorPhotoNSuperSim_PositionSys -n 1 -src 88y -set 10 -ngen 3700000 -prim -otype root -template fluxcount_PNZip.mac.template -shift 0 -srcZ 39 -srcA 88 -exe

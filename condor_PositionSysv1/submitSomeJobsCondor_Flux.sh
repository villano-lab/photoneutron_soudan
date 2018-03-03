#!/bin/sh


#3/2/18
# submit a flux job for the zips 
./condorPhotoNSuperSim_PositionSys -n 100 -src 88y -set 10 -ngen 3700000 -prim -otype root -template fluxcount_PNZip.mac.template -shift 0 -srcZ 39 -srcA 88 -exe

#!/bin/sh

# loop to submit many jobs 
for i in $(seq 1 100) 
do
  ./condorPhotoNSuperSim_PositionSys -n 1 -src 88y -set 0 -ngen 37000000 -prim -otype root -template soudan_photon_gamma_possys.mac.template -shift 0 -srcZ 39 -srcA 88 -exe
done


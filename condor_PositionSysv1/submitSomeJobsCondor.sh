#!/bin/sh

# loop to submit many jobs 
#for i in $(seq 1 100) 
#do
#  ./condorPhotoNSuperSim_PositionSys -n 1 -src 88y -set 1 -ngen 37000000 -prim -otype root -merge -template soudan_photon_gamma_possys.mac.template -shift 3.14 -srcZ 39 -srcA 88 -exe
#done

# a loop for primary data recording 
#for i in -10 -9 -8 -7 -6 -5 -4 -3 -2 -1 0 1 2 3 4 5 6 7 8 9 10 
#do
#  ./condorPhotoNSuperSim_PositionSys -n 1 -src 88y -set 1 -ngen 10000 -prim -otype ascii -template soudan_photon_gamma_possys.mac.template -shift ${i} -srcZ 39 -srcA 88 -exe
#done

# loop to submit many jobs 
#for i in $(seq 1 2) 
#do
#  for j in -13.4 -12.56 -11.73 -10.89 -10.05 -9.21 -8.38 -7.54 -6.7 -5.86 -5.03 -4.19 -3.35 -2.51 -1.68 -0.84 0 0.84 1.68 2.51 3.35 4.19 5.03 5.86 6.7
#  do 
#    ./condorPhotoNSuperSim_PositionSys -n 1 -src 88y -set 1 -ngen 37000000 -prim -otype root -merge -template soudan_photon_gamma_possys.mac.template -shift ${j} -srcZ 39 -srcA 88 -exe
#  done
#done

# loop with lateral displacement as well
# loop to submit many jobs 
#for i in -2 -1 1 2 3
#do
#  for j in -13.4 -12.56 -11.73 -10.89 -10.05 -9.21 -8.38 -7.54 -6.7 -5.86 -5.03 -4.19 -3.35 -2.51 -1.68 -0.84 0 0.84 1.68 2.51 3.35 4.19 5.03 5.86 6.7
#  do 
#    ./condorPhotoNSuperSim_PositionSys -n 1 -src 88y -set 2 -ngen 37000000 -prim -otype root -merge -template soudan_photon_gamma_2Dpossys.mac.template -shift ${j} -lshift ${i} -srcZ 39 -srcA 88 -exe
#  done
#done

# loop with lateral displacement as well
# small jobs to record the primaries
# loop to submit many jobs 
#for i in -2 -1 0 1 2 3
#do
#  for j in -13.4 -12.56 -11.73 -10.89 -10.05 -9.21 -8.38 -7.54 -6.7 -5.86 -5.03 -4.19 -3.35 -2.51 -1.68 -0.84 0 0.84 1.68 2.51 3.35 4.19 5.03 5.86 6.7
#  do 
#    ./condorPhotoNSuperSim_PositionSys -n 1 -src 88y -set 2 -ngen 1000 -prim -otype ascii -template soudan_photon_gamma_2Dpossys.mac.template -shift ${j} -lshift ${i} -srcZ 39 -srcA 88 -exe
#  done
#done

# loop with zero lateral displacement as well (new SuperSim version)
# loop to submit many jobs 
#for i in 0
#do
#  for j in -13.4 -12.56 -11.73 -10.89 -10.05 -9.21 -8.38 -7.54 -6.7 -5.86 -5.03 -4.19 -3.35 -2.51 -1.68 -0.84 0 0.84 1.68 2.51 3.35 4.19 5.03 5.86 6.7
#  do 
#    ./condorPhotoNSuperSim_PositionSys -n 1 -src 88y -set 3 -ngen 37000000 -prim -otype root -merge -template soudan_photon_gamma_2Dpossys_diagnosticseed.mac.template -shift ${j} -lshift ${i} -srcZ 39 -srcA 88 -exe
#  done
#done

#2/8/18
#do just the along bridge shift with updated code develop tag elog/445
# loop to submit many jobs 
for i in $(seq 1 2) 
do
  for j in -13.4 -12.56 -11.73 -10.89 -10.05 -9.21 -8.38 -7.54 -6.7 -5.86 -5.03 -4.19 -3.35 -2.51 -1.68 -0.84 0 0.84 1.68 2.51 3.35 4.19 5.03 5.86 6.7
  do 
    ./condorPhotoNSuperSim_PositionSys -n 1 -src 88y -set 4 -ngen 37000000 -prim -otype root -merge -template soudan_photon_gamma_2Dpossys.mac.template -shift ${j} -srcZ 39 -srcA 88 -exe
  done
done

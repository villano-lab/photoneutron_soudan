#!/bin/sh


source ./setup.sh
for i in -13.4 -10.05 -6.7 -3.35 0.0 3.35 6.7  
do 
  echo ${i}
  #compute the box position
  BP=`bc <<< "${i} + 8.15"`
  BPUS=`sed -e 's|\.|_|g' <<< "${i}"` 
  sed -e 's|OFFSETVALUE|'${BP}'|g' -e 's|SHIFTVALUE|'${BPUS}'|g' < draw_vert_photoN_shift.mac.template > .SideView.mac 
  CDMS_SimDraw .SideView.mac
  rm .SideView.mac
  convert -density 300 pics/photon-side-shift_${BPUS}_0.eps pics/photon-side-shift_${BPUS}_0.png
  rm pics/photon-side-shift_${BPUS}_0.eps
  LIST="${LIST} pics/photon-side-shift_${BPUS}_0.png"
done
echo ${LIST}
convert -delay 100 -loop 0 ${LIST} Soudan_PhotoNeutron_shiftAnimation_side.gif

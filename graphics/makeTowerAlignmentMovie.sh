#!/bin/sh

DEL=$1

source ./setup.sh
for i in $(seq 10 ${DEL} 100)
do 
  echo ${i}
  sed -e 's|HEIGHTSHIFT|'${i}'|g' < scan_ToptoBot.mac.template > .ToptoBotscan.mac 
  CDMS_SimDraw .ToptoBotscan.mac
  rm .ToptoBotscan.mac
  convert -density 300 pics/vertmovie_${i}_0.eps pics/vertmovie_${i}_0.png
  rm pics/vertmovie_${i}_0.eps
done
convert -delay 10 -loop 0 pics/vertmovie*.png Soudan_PhotoNeutron_vertAnimation_del${DEL}.gif
rm pics/vertmovie*.png

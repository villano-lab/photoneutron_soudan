#!/bin/sh

#a script to run jobs until we see trajectories of interest (any that hit the detectors)
CDMS_GGSim viewTrajectoriesWHits.mac

#figure out the date
SUFFIX=`date +"10"%y%m%d"_1000"`
PICSUFFIX=`date +%y%m%d"_"%H%M%S"_"%N`
echo ${SUFFIX}
echo ${PICSUFFIX}

TEMP=`cat trajectorySupersim_${SUFFIX}.txt |grep -v 'Run\|D3'|awk 'END{print NR-1;}'`
echo ${TEMP}

if [ ${TEMP} != "0" ]
then
  mv trajectory-shift_6_7_0.eps trajectory_pics/trajectory-shift_6_7_${PICSUFFIX}_100Tracks.eps
fi

rm trajectorySupersim_${SUFFIX}.txt

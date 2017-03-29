#!/bin/sh

DATASETID=$1
RUN=$2
SOURCE=$3
NEV=$4
OTYPE="root"
TEMPLATEFILE=$5
SHIFT=$6
LSHIFT=$7
SOURCEZ=$8
SOURCEA=$9
JOBID=${10}
SOURCEDIR=${SOURCE}

if [ "${OTYPE}" = "root" ];
then
  PRIMOUT="false"
  DATAOUT="false"
  TREEOUT="true"
  SOURCEDIR=${SOURCE}"root"
fi


RUNNUM4DIG=`printf "%04d" ${RUN}`
DATASETIDHEX=`printf "0x%04x" ${DATASETID}`
DATE=`date +%y%m%d"_"%H%M%S"_"%N`
DAY=`date +%y%m%d`

#echo ${DATE}

#compute the box position
BOXPOSITION=`bc <<< "${SHIFT} + 8.15"`
POSX=`bc <<< "-1*${LSHIFT}*0.766 -1.1637"` #d*cos(40deg) N-MISC-16-001
POSY=`bc <<< "${LSHIFT}*0.6428 +0.8148"` #d*sin(40deg) +x N-MISC-16-001

#echo ${JOBID}
if [ -z "${JOBID}" ];
then
  #echo jobid  blank
  RUNDIR=.
  RELPATH=./
else
  #echo jobid NOT blank
  if [ ! -d "${JOBID}" ];
  then
    mkdir -p /data/chocula/villaa/PhotoN_SuperSim/${JOBID}
    ln -s /data/chocula/villaa/PhotoN_SuperSim/${JOBID} ${JOBID}
  fi
  RUNDIR=${JOBID}
  RELPATH=../
fi

#make the simulation macro file (USE ROOT output)
sed -e 's|SOURCENAME|'${SOURCE}'|g' -e 's|THESETID|'${DATASETIDHEX}'|g' -e 's|NEVENTS|'${NEV}'|g' -e 's|RUNNUMBER|'${RUN}'|g' -e 's|TIMEOFDAY|'${DATE}'|g' -e 's|PRIMOUT|'${PRIMOUT}'|g' -e 's|DATAOUT|'${DATAOUT}'|g' -e 's|TREEOUT|'${TREEOUT}'|g' -e 's|DATADIR|'${SOURCEDIR}'|g' -e 's|BOXPOSITION|'${BOXPOSITION}'|g' -e 's|SOURCEZ|'${SOURCEZ}'|g' -e 's|SOURCEA|'${SOURCEA}'|g' -e 's|BOXSHIFT|'${SHIFT}'|g' -e 's|POSX|'${POSX}'|g' -e 's|POSY|'${POSY}'|g' -e 's|LATERALSHIFT|'${LSHIFT}'|g'  < ${TEMPLATEFILE} > 'macros/'${SOURCE}'_'${DATASETIDHEX}'_SHIFT'${SHIFT}'_LSHIFT'${LSHIFT}'_'${RUN}'_'${DATE}'.mac'

#run the simulation job
#cat macros/${SOURCE}_${DATASETIDHEX}_${RUN}_${DATE}.mac
cd ${RUNDIR}
pwd
CDMS_GGSim ${RELPATH}macros/${SOURCE}_${DATASETIDHEX}_SHIFT${SHIFT}_LSHIFT${LSHIFT}_${RUN}_${DATE}.mac

#upon return run the ROOT output condensation
#/home/phys/villaa/supersim/CDMSscripts/analysis/CombineTrees /data/chocula/villaa/PhotoN_SuperSim/${SOURCEDIR}/${SOURCE}_SetID${DATASETIDHEX}_shift${SHIFT}_lshift${LSHIFT}_p${NEV}_${DATE}_10${DAY}_${RUNNUM4DIG}.root 
#/home/phys/villaa/supersim_analysis/CombineTrees /data/chocula/villaa/PhotoN_SuperSim/${SOURCEDIR}/${SOURCE}_SetID${DATASETIDHEX}_shift${SHIFT}_lshift${LSHIFT}_p${NEV}_${DATE}_10${DAY}_${RUNNUM4DIG}.root 

#remove the original ROOT file
#rm /data/chocula/villaa/PhotoN_SuperSim/${SOURCEDIR}/${SOURCE}_SetID${DATASETIDHEX}_shift${SHIFT}_lshift${LSHIFT}_p${NEV}_${DATE}_10${DAY}_${RUNNUM4DIG}.root 

#now just add the tree for metaData
#root -l -b addMetaData.C\(\"/data/chocula/villaa/PhotoN_SuperSim/${SOURCEDIR}/${SOURCE}_SetID${DATASETIDHEX}_shift${SHIFT}_lshift${LSHIFT}_p${NEV}_${DATE}_10${DAY}_${RUNNUM4DIG}_merged.root\"\); 

#!/bin/sh

DATASETID=$1
RUN=$2
SOURCE=$3
NEV=$4
OTYPE="root"
TEMPLATEFILE=$5

if [ "${OTYPE}" = "root" ];
then
  PRIMOUT="true"
  DATAOUT="false"
  TREEOUT="true"
  SOURCEDIR=${SOURCE}"root"
fi


RUNNUM4DIG=`printf "%04d" ${RUN}`
DATASETIDHEX=`printf "0x%04x" ${DATASETID}`
DATE=`date +%y%m%d"_"%H%M%S"_"%N`
DAY=`date +%y%m%d`

#echo ${DATE}

#make the simulation macro file (USE ROOT output)
sed -e 's|SOURCE|'${SOURCE}'|g' -e 's|THESETID|'${DATASETIDHEX}'|' -e 's|NEVENTS|'${NEV}'|' -e 's|RUNNUMBER|'${RUN}'|' -e 's|TIMEOFDAY|'${DATE}'|' -e 's|PRIMOUT|'${PRIMOUT}'|' -e 's|DATAOUT|'${DATAOUT}'|' -e 's|TREEOUT|'${TREEOUT}'|' -e 's|DATADIR|'${SOURCEDIR}'|'  < ${TEMPLATEFILE}  > 'macros/'${SOURCE}'_'${DATASETIDHEX}'_'${RUN}'_'${DATE}'.mac'

#run the simulation job
#cat macros/${SOURCE}_${DATASETIDHEX}_${RUN}_${DATE}.mac
CDMS_GGSim macros/${SOURCE}_${DATASETIDHEX}_${RUN}_${DATE}.mac

#upon return run the ROOT output condensation
/home/phys/villaa/supersim/CDMSscripts/analysis/CombineTrees /data/chocula/villaa/PhotoN_SuperSim/${SOURCEDIR}/${SOURCE}_SetID${DATASETIDHEX}_p${NEV}_${DATE}_10${DAY}_${RUNNUM4DIG}.root 

#remove the original ROOT file
rm /data/chocula/villaa/PhotoN_SuperSim/${SOURCEDIR}/${SOURCE}_SetID${DATASETIDHEX}_p${NEV}_${DATE}_10${DAY}_${RUNNUM4DIG}.root 

#remove the text primary file 
rm /data/chocula/villaa/PhotoN_SuperSim/${SOURCEDIR}/${SOURCE}_SetID${DATASETIDHEX}_p${NEV}_${DATE}_10${DAY}_${RUNNUM4DIG}_Primary.txt 


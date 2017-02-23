#!/bin/sh

DATASETID=$1
RUN=$2
SOURCE=$3
NEV=$4
OTYPE="root"
TEMPLATEFILE=$5
SHIFT=$6
SOURCEZ=$7
SOURCEA=$8
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

#make the simulation macro file (USE ROOT output)
sed -e 's|SOURCENAME|'${SOURCE}'|g' -e 's|THESETID|'${DATASETIDHEX}'|g' -e 's|NEVENTS|'${NEV}'|g' -e 's|RUNNUMBER|'${RUN}'|g' -e 's|TIMEOFDAY|'${DATE}'|g' -e 's|PRIMOUT|'${PRIMOUT}'|g' -e 's|DATAOUT|'${DATAOUT}'|g' -e 's|TREEOUT|'${TREEOUT}'|g' -e 's|DATADIR|'${SOURCEDIR}'|g' -e 's|BOXPOSITION|'${BOXPOSITION}'|g' -e 's|SOURCEZ|'${SOURCEZ}'|g' -e 's|SOURCEA|'${SOURCEA}'|g' -e 's|BOXSHIFT|'${SHIFT}'|g'  < ${TEMPLATEFILE} > 'macros/'${SOURCE}'_'${DATASETIDHEX}'_SHIFT'${SHIFT}'_'${RUN}'_'${DATE}'.mac'

#run the simulation job
#cat macros/${SOURCE}_${DATASETIDHEX}_${RUN}_${DATE}.mac
CDMS_GGSim macros/${SOURCE}_${DATASETIDHEX}_SHIFT${SHIFT}_${RUN}_${DATE}.mac

#upon return run the ROOT output condensation
/home/phys/villaa/supersim/CDMSscripts/analysis/CombineTrees /data/chocula/villaa/PhotoN_SuperSim/${SOURCEDIR}/${SOURCE}_SetID${DATASETIDHEX}_shift${SHIFT}_p${NEV}_${DATE}_10${DAY}_${RUNNUM4DIG}.root 

#remove the original ROOT file
rm /data/chocula/villaa/PhotoN_SuperSim/${SOURCEDIR}/${SOURCE}_SetID${DATASETIDHEX}_shift${SHIFT}_p${NEV}_${DATE}_10${DAY}_${RUNNUM4DIG}.root 

#now just add the tree for metaData
root -l -b addMetaData.C\(\"/data/chocula/villaa/PhotoN_SuperSim/${SOURCEDIR}/${SOURCE}_SetID${DATASETIDHEX}_shift${SHIFT}_p${NEV}_${DATE}_10${DAY}_${RUNNUM4DIG}_merged.root\"\); 

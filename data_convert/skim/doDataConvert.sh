#!/bin/sh

FILE=$1
NUM=$2

RFILE=`basename ${FILE}`

IDSTRING=`echo ${RFILE} |awk 'BEGIN{FS="."}{print $1;}'`

echo ${RFILE}
echo ${IDSTRING}

#use the escape info to write the tree
root -l -b -q skimTest_wVoltage.C\(14,\"ybe\",10,5,\"tiny\"\);

#remove the escape info file
#rm hitevents_${IDSTRING}.txt

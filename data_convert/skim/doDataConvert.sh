#!/bin/sh

ZIP=$1
DSET=$2
NFILES=$3
OFFSET=$4
POSTFIX=$5
DSETID=${6:-5}

#use the escape info to write the tree
root -l -b -q skim_wVoltage.C\(${ZIP},\"${DSET}\",${NFILES},${OFFSET},${DSETID},\"${POSTFIX}\"\);


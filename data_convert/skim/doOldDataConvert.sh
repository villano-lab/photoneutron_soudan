#!/bin/sh

ZIP=$1
DSET=$2
NFILES=$3
POSTFIX=$4

#use the escape info to write the tree
root -l -b -q skimTest_wVoltage.C\(${ZIP},\"${DSET}\",${NFILES},4,\"${POSTFIX}\"\);


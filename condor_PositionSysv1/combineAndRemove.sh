#!/bin/sh

FILENAME=$1

#upon return run the ROOT output condensation
/home/phys/villaa/supersim/CDMSscripts/analysis/CombineTrees ${FILENAME} 

#remove the original ROOT file
rm ${FILENAME} 


#!/bin/sh

FILENAME=$1

BASEFILENAME=`basename ${FILENAME}`

sed -e 's|THEARGUMENTS|'${FILENAME}'|g' -e 's|IDENTIFIER|'${BASEFILENAME}'|g' < condor-SuperSimConvert > .condorconvert

#cat .condorconvert
condor_submit .condorconvert

rm .condorconvert

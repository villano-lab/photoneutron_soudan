#!/bin/sh

FILENAME=$1
SOURCE=$2

FILENAME=`basename ${FILENAME}`

sed -e 's|THEARGUMENTS|'${FILENAME}'|g' -e 's|IDENTIFIER|'${FILENAME}'|g' -e 's|SOURCE|'${SOURCE}'|g' < condor-SuperSimConvert > .condorconvert

condor_submit .condorconvert

rm .condorconvert

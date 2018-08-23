#!/bin/sh

SOURCE=$1
DIR=/data/chocula/villaa/PhotoN_SuperSim/${SOURCE}root/
DSET=0x0005
NFMAX=10

NUM=`ls ${DIR} |grep .root|grep ${DSET}|grep ${SOURCE}|grep _merge|awk 'END{print NR}'`

NG=`echo \($NUM - \(${NUM} % ${NFMAX}\)\)/${NFMAX} |bc`
MOD=`echo \(${NUM} % ${NFMAX}\) |bc`

echo $NG
echo $MOD

#!/bin/sh

SOURCE=$1
ZIP=$2
DIR=/data/chocula/villaa/PhotoN_SuperSim/${SOURCE}root/
DSET=0x0005
NFMAX=100


EXE=no

input=($@)

for((i=0; i<$#; i++))
do

  if [ "${input[i]}" = "-exe" ];
  then
    EXE=yes
  fi

done

echo ${EXE}

NUM=`ls ${DIR} |grep .root|grep ${DSET}|grep ${SOURCE}|grep _merge|awk 'END{print NR}'`

NG=`echo \($NUM - \(${NUM} % ${NFMAX}\)\)/${NFMAX} |bc`
MOD=`echo \(${NUM} % ${NFMAX}\) |bc`

echo $NG
echo $MOD

if [ ${MOD} -ne 0 ] 
then
  NG=$((NG+1))
fi
echo $NG
echo $MOD


idx=$((NG-1))
for i in $(seq 0 ${idx})
do
  echo ${i}
  off=$((100*i))
  sed -e 's|IDENTIFIER|'${DSET}'_'${i}'-'${NFMAX}'|g' -e 's|ARGUMENTS|'${ZIP}' '${SOURCE}' '${NFMAX}' '${off}' '${i}'_max'${NFMAX}'|g' < condor/condor-DataConvert > testcondor
  cat testcondor
  if [ ${EXE} != "yes" ]
  then
    echo NOT SUBMITTING
  else
    condor_submit testcondor
  fi
  rm testcondor
done

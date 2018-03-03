#!/bin/sh

date > lastsubmit.txt
condor_q -wide -nobatch |grep running |awk 'BEGIN{jobs=0}{jobs=$9+$11+0; if(jobs<10){

system("date > lastsubmit.txt");
system("env |grep G4 >> lastsubmit.txt");
system("pwd >> lastsubmit.txt");
system("./submitSomeJobsCondor.sh >> lastsubmit.txt");


} }'

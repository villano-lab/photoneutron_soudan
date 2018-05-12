#!/bin/sh

date > lastsubmit.txt
condor_q -wide -nobatch |grep running |awk 'BEGIN{jobs=0}{jobs=$1+0; print jobs; if(jobs<10){

system("date > lastsubmit.txt");
system("env |grep G4 >> lastsubmit.txt");
system("pwd >> lastsubmit.txt");
system("./submitSomeJobsCondor.sh >> lastsubmit.txt");


} }'

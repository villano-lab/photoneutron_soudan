#!/bin/sh

condor_q -wide -nobatch |grep running |awk 'BEGIN{jobs=0}{jobs=$9+0; if(jobs<10){

system("env |grep G4 > lastsubmit.txt");
system("pwd >> lastsubmit.txt");
system("./submitSomeJobsCondor.sh >> lastsubmit.txt");
system("./submitSomeJobsCondor.sh >> lastsubmit.txt");


} }'

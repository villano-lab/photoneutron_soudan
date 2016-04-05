/*==================supersima2root.cpp================================= 
   
      PROGRAMMER:  Anthony Villano 03/13/16

      UPDATES:   


      PURPOSE:  Take in a Geant4 CDMS SuperSim standard output .txt file and create
                a ROOT Tree.  The tree should either be organized by event or by hit.

              
      INPUT:      

      OUTPUT:   
              
======================================================================*/
#include <iostream> 
#include <iomanip> 
#include <sstream>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <string> 
#include <stdlib.h>

#include "Riostream.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

#include "getSimEvent.h"
using namespace std;

#define MAXHIT 1000000

TTree *RCtree(string,vector<Long64_t>&,vector<Double_t>&,vector<Long64_t>&,Long64_t*,Double_t*,
bool evtgrp=false);
bool setVars(const vector<double>&,vector<Long64_t>&,vector<Double_t>&,vector<Long64_t>&);
bool setArrays(const vector<vector<double> >&,vector<Long64_t>&,vector<Double_t>&,vector<Long64_t>&,
Long64_t*, Double_t*);
string NumtoStr(int, int);
string makeOutFile(string,const vector<string>&,int,int numtags=3);
int main(int argc, char *argv[])
{
   string rep;
   string infile = "test.txt";
   string dir = "./";
   string outfile = "test";
   int ntags=3; //number of tags attached to ascii file name
   int autoflush=0;
   bool evtgrp=false;

   for(int i=0; i<argc; i++)
   {
      rep=argv[i];
      if(rep=="-in")
        infile = argv[i+1];
      if(rep=="-o")
        outfile = argv[i+1];
      if(rep=="-dir")
        dir = argv[i+1];
      if(rep=="-ntag")
        ntags=atoi(argv[i+1]);
      if(rep=="-flush")
        autoflush=atoi(argv[i+1]);
      if(rep=="-E")
        evtgrp=true;
   }

   gROOT->Reset();
   
   // declare contents of data columns
   //Int_t elength;
   //Long64_t Date, Time, Nanoseconds, Run, EventNum, DetType, DetNum, TrkStep, Parent, PType;
   //Float_t Empty, KE, Edep, Yield, Depth, Xmom3, Ymom3, Zmom3, X3, Y3, Z3, Time3, Xmom1, Ymom1, Zmom1, X1, Y1, Z1, Time1 ;

   vector<Long64_t> data0(10,0);
   vector<Double_t> data1(12,0.0);
   //header part for event-mode writing
   vector<Long64_t> data_head(5,0);
   Long64_t *evdata0;
   Double_t *evdata1;
   evdata0 = (Long64_t*) malloc(5*MAXHIT*sizeof *evdata0);
   evdata1 = (Double_t*) malloc(12*MAXHIT*sizeof *evdata1);


   //use event dispenser for simuation events
   getSimEvent simevent(infile);
   if(!simevent.isConnect()){
     cerr << "File not read properly, exit without write." << endl;
     exit(1);
   }
   simevent.event();


   //some counters
   int currentLine=0,currentFile=0;

   //make the outputfile
   string outfilename;
   outfilename=(dir+makeOutFile(outfile,simevent.getRunTags(),currentFile,ntags));

   // create tree and the ROOT file to store it
   TFile *rootfile = new TFile(outfilename.c_str(),"RECREATE");
   TTree *data;
   
   data = RCtree("datatree",data0,data1,data_head,evdata0,evdata1,evtgrp); 

       //data->Branch("nhits",&elength,"nhits/I");
       //data->Branch("ke",evke,"ke[nhits]/D");
   data->SetAutoFlush(autoflush);
   if(data->GetAutoFlush()>0)
     cout << "AutoFlush is set to: " << data->GetAutoFlush() << " events" << endl;
   else if(data->GetAutoFlush()<0)
     cout << "AutoFlush is set to: " << -data->GetAutoFlush()/1000000 << " Mb" << endl;
   else
     cout << "AutoFlush is OFF " << endl;
   //loop over all events
   while(simevent.isConnect())
   {
     if(!evtgrp){
       for(int i=0;i<simevent.data.size();i++){
         //set pointers
         if(setVars(simevent.data[i],data0,data1,data_head)){
           for(int j=0;j<min(3,(int)simevent.getRunTags().size());j++){
             data0[2-j]=atol(simevent.getRunTags()[j].c_str());
             data_head[2-j]=atol(simevent.getRunTags()[j].c_str());
           }
           data_head[4]=(Long64_t)simevent.data.size();
           data->Fill();
         }
         currentLine++;
       }
     }
     else{
       //set pointers
       if(setArrays(simevent.data,data0,data1,data_head,evdata0,evdata1)){
         for(int j=0;j<min(3,(int)simevent.getRunTags().size());j++){
           data0[2-j]=atol(simevent.getRunTags()[j].c_str());
           data_head[2-j]=atol(simevent.getRunTags()[j].c_str());
         }
         data_head[4]=(Long64_t)simevent.data.size();
         data->Fill();
       }
       currentLine+=simevent.data.size();
     }
     if(currentLine>1000000){
       cout << "Hits written to file " << currentFile << ": " << currentLine << endl;
       //close this file
       // write the tree to the file and close it for now
       rootfile->Write();
       rootfile->Close();
       delete rootfile;

       //destroy tree
       //this (below causes segfault, I guess I don't understand)
       //why I don't have to destroy, and I wonder if I have memory leak
       //ANSWER: the tree is part of the TFile in this situation, deleting
       //the TFile and the TTree is redundant and will attempt access to
       //memory locations that no longer exist.  see:
       //http://www-glast.slac.stanford.edu/software/root/howto/faq.htm#deleteTTree
       //delete data;

       //open new file
       currentFile++;
       outfilename.clear();
       outfilename=(dir+makeOutFile(outfile,simevent.getRunTags(),currentFile,ntags));
       //rootfile = TFile::Open(outfilename.c_str(),"RECREATE");
       rootfile = new TFile(outfilename.c_str(),"RECREATE");
       //make new tree
       data = RCtree("datatree",data0,data1,data_head,evdata0,evdata1,evtgrp); 
       data->SetAutoFlush(autoflush);
       currentLine=0;
     }
     simevent.event();
   }


   cout << "Hits written to file " << currentFile << ": " << currentLine << endl;
   // write the tree to the file and close it for now
   rootfile->Write();
   rootfile->Close();
   delete rootfile;
   return 0;
}
TTree *RCtree(string treename,vector<Long64_t> &data0,vector<Double_t> &data1,
vector<Long64_t> &data_head,
Long64_t *evdata0,
Double_t *evdata1,
bool evtgrp)
{
   TTree *datatree = new TTree(treename.c_str(),"data from ASCII file");

   //check if data0 and data1 have the right size if not
   //return null pointer
   if(data0.size()!=10 || data1.size()!=19 || data_head.size()!=6){
     cout << "Wrong vector sizes for storage." << endl;
     return 0;
   }
   //Long64_t Date, Time, Nanoseconds, Run, EventNum, DetType, DetNum, TrkStep, Parent, PType;
   //Float_t Empty, KE, Edep, Yield, Depth, Xmom3, Ymom3, Zmom3, X3, Y3, Z3, Time3, Xmom1, Ymom1, Zmom1, X1, Y1, Z1, Time1 ;
  
   if(!evtgrp){ 
     // create branches in tree for each data column
     datatree->Branch("Date",&data0[0],"Date/L");
     datatree->Branch("Time",&data0[1],"Time/L");
     datatree->Branch("Nanoseconds",&data0[2],"Nanoseconds/L");
     datatree->Branch("Run",&data0[3],"Run/L");
     datatree->Branch("EventNum",&data0[4],"EventNum/L");
     datatree->Branch("DetType",&data0[5],"DetType/L");
     datatree->Branch("DetNum",&data0[6],"DetNum/L");
     datatree->Branch("TrkStep",&data0[7],"TrkStep/L");
     datatree->Branch("Parent",&data0[8],"Parent/L");
     datatree->Branch("PType",&data0[9],"PType/L");
     datatree->Branch("Empty",&data1[0],"Empty/D");
     datatree->Branch("KE",&data1[1],"KE/D");
     datatree->Branch("Edep",&data1[2],"Edep/D");
     datatree->Branch("Yield",&data1[3],"Yield/D");
     datatree->Branch("Depth",&data1[4],"Depth/D");
     datatree->Branch("Xmom3",&data1[5],"Xmom3/D");
     datatree->Branch("Ymom3",&data1[6],"Ymom3/D");
     datatree->Branch("Zmom3",&data1[7],"Zmom3/D");
     datatree->Branch("X3",&data1[8],"X3/D");
     datatree->Branch("Y3",&data1[9],"Y3/D");
     datatree->Branch("Z3",&data1[10],"Z3/D");
     datatree->Branch("Time3",&data1[11],"Time3/D");
     datatree->Branch("Xmom1",&data1[12],"Xmom1/D");
     datatree->Branch("Ymom1",&data1[13],"Ymom1/D");
     datatree->Branch("Zmom1",&data1[14],"Zmom1/D");
     datatree->Branch("X1",&data1[15],"X1/D");
     datatree->Branch("Y1",&data1[16],"Y1/D");
     datatree->Branch("Z1",&data1[17],"Z1/D");
     datatree->Branch("Time1",&data1[18],"Time1/D");
   }
   else{
     //create branches which store vectors
     datatree->Branch("Date",&data_head[0],"Date/L");
     datatree->Branch("Time",&data_head[1],"Time/L");
     datatree->Branch("Nanoseconds",&data_head[2],"Nanoseconds/L");
     datatree->Branch("Run",&data_head[3],"Run/L");
     datatree->Branch("EventNum",&data_head[4],"EventNum/L");

     datatree->Branch("nhits",&data_head[5],"nhits/L");
     datatree->Branch("DetType",&evdata0[0*MAXHIT],"DetType[nhits]/L");
     datatree->Branch("DetNum",&evdata0[1*MAXHIT],"DetNum[nhits]/L");
     datatree->Branch("TrkStep",&evdata0[2*MAXHIT],"TrkStep[nhits]/L");
     datatree->Branch("Parent",&evdata0[3*MAXHIT],"Parent[nhits]/L");
     datatree->Branch("PType",&evdata0[4*MAXHIT],"PType[nhits]/L");
     datatree->Branch("KE",&evdata1[0*MAXHIT],"KE[nhits]/D");
     datatree->Branch("Edep",&evdata1[1*MAXHIT],"Edep[nhits]/D");
     datatree->Branch("Yield",&evdata1[2*MAXHIT],"Yield[nhits]/D");
     datatree->Branch("Depth",&evdata1[3*MAXHIT],"Depth[nhits]/D");
     datatree->Branch("Xmom3",&evdata1[4*MAXHIT],"Xmom3[nhits]/D");
     datatree->Branch("Ymom3",&evdata1[5*MAXHIT],"Ymom3[nhits]/D");
     datatree->Branch("Zmom3",&evdata1[6*MAXHIT],"Zmom3[nhits]/D");
     datatree->Branch("X3",&evdata1[7*MAXHIT],"X3[nhits]/D");
     datatree->Branch("Y3",&evdata1[8*MAXHIT],"Y3[nhits]/D");
     datatree->Branch("Z3",&evdata1[9*MAXHIT],"Z3[nhits]/D");
     datatree->Branch("Time3",&evdata1[10*MAXHIT],"Time3[nhits]/D");
     datatree->Branch("Xmom1",&evdata1[11*MAXHIT],"Xmom1[nhits]/D");
     datatree->Branch("Ymom1",&evdata1[12*MAXHIT],"Ymom1[nhits]/D");
     datatree->Branch("Zmom1",&evdata1[13*MAXHIT],"Zmom1[nhits]/D");
     datatree->Branch("X1",&evdata1[14*MAXHIT],"X1[nhits]/D");
     datatree->Branch("Y1",&evdata1[15*MAXHIT],"Y1[nhits]/D");
     datatree->Branch("Z1",&evdata1[16*MAXHIT],"Z1[nhits]/D");
     datatree->Branch("Time1",&evdata1[17*MAXHIT],"Time1[nhits]/D");

   }
   return datatree;
}
bool setVars(const vector<double> &event,vector<Long64_t> &data0,vector<Double_t> &data1,
vector<Long64_t> &data_head)
{

  if(event.size()!=22 || data0.size()!=10 || data1.size()!=19 || data_head.size()!=6)
    return 0;

  data0[4]=(Long64_t)event[0];
  data0[5]=(Long64_t)700000; //detector type
  data0[6]=(Long64_t)event[1];
  data0[7]=(Long64_t)event[2];
  data0[8]=(Long64_t)event[3];
  data0[9]=(Long64_t)event[4];
  data1[0]=(Double_t)0;
  data1[1]=(Double_t)event[5];
  data1[2]=(Double_t)event[6];
  data1[3]=(Double_t)event[7];
  data1[4]=(Double_t)0;
  data1[5]=(Double_t)event[8];
  data1[6]=(Double_t)event[9];
  data1[7]=(Double_t)event[10];
  data1[8]=(Double_t)event[11];
  data1[9]=(Double_t)event[12];
  data1[10]=(Double_t)event[13];
  data1[11]=(Double_t)event[14];
  data1[12]=(Double_t)event[15];
  data1[13]=(Double_t)event[16];
  data1[14]=(Double_t)event[17];
  data1[15]=(Double_t)event[18];
  data1[16]=(Double_t)event[19];
  data1[17]=(Double_t)event[20];
  data1[18]=(Double_t)event[21];

  data_head[4]=(Long64_t)event[0];

  return 1;
}
bool setArrays(const vector<vector<double> > &event,vector<Long64_t> &data0,vector<Double_t> &data1,
vector<Long64_t> &data_head,
Long64_t *evdata0,Double_t *evdata1)
{

  if(data0.size()!=10 || data1.size()!=19 || data_head.size()!=6)
    return 0;

  if(event.size()==0)
    return 0;

  //first set the non-array parameters
  bool toplev=setVars(event[0],data0,data1,data_head);

  if(!toplev)
    return 0;

  //now fill the arrays
  for(int i=0;i<(int)event.size();i++){
     evdata0[0*MAXHIT+i]=(Long64_t)event[i][1];
     evdata0[1*MAXHIT+i]=(Long64_t)event[i][2];
     evdata0[2*MAXHIT+i]=(Long64_t)event[i][3];
     evdata0[3*MAXHIT+i]=(Long64_t)event[i][4];
     evdata1[0*MAXHIT+i]=(Double_t)event[i][5];
     evdata1[1*MAXHIT+i]=(Double_t)event[i][6];
     evdata1[2*MAXHIT+i]=(Double_t)event[i][7];
     evdata1[3*MAXHIT+i]=(Double_t)event[i][8];
     evdata1[4*MAXHIT+i]=(Double_t)event[i][9];
     evdata1[5*MAXHIT+i]=(Double_t)event[i][10];
     evdata1[6*MAXHIT+i]=(Double_t)event[i][11];
     evdata1[7*MAXHIT+i]=(Double_t)event[i][12];
     evdata1[8*MAXHIT+i]=(Double_t)event[i][13];
     evdata1[9*MAXHIT+i]=(Double_t)event[i][14];
     evdata1[10*MAXHIT+i]=(Double_t)event[i][15];
     evdata1[11*MAXHIT+i]=(Double_t)event[i][16];
     evdata0[4*MAXHIT+i]=(Long64_t)event[i][17];
  }

  return 1;
}
string NumtoStr(int number, int length)
{
  //G4String name;
  std::ostringstream oss;

  oss << std::setfill('0');
  oss << std::setw(length) << number;

  return oss.str();
}
string makeOutFile(string outfile,const vector<string> &tags,int currentFile,int numtags)
{
   //make first outfile name
   for(int i=0;i<min(numtags,(int)tags.size());i++)
      outfile+=("_"+tags[i]);
   outfile+=("_"+NumtoStr(currentFile,3)+".root");

   return outfile;
}

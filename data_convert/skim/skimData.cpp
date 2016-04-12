/*==================skimData.cpp======================================= 
   
      PROGRAMMER:  Anthony Villano 03/19/16

      UPDATES:      
       

      PURPOSE: Make a compiled code to skim the data to see if it's faster
               than the root macro in this directory cullEvents.C.
              
======================================================================*/
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <iomanip>
#include <fstream>
#include <sstream>


//ROOT stuff
#include "Rtypes.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1D.h"
#include "TEventList.h"
#include "TEntryList.h"
#include "TSystem.h"

using namespace std;

TChain *chainPhotoNSuperSim(int &nev,int n=-1,int datasetno=0,string source="ybe",string dir="/data/chocula/villaa/PhotoN_SuperSim/yberoot",string treename="mcmerged");
TTree *projectSumDepEv(TChain *ch,string cut="",int maxev=1000000,int zip=1);
TTree *projectSumDepEv_AllInfo(TChain *ch,string cut="",int maxev=1000000,int zip=1);
int main(int argc, char *argv[])
{
   string rep; 
   string outfile="test.root";
   string outdir="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/";
   string indir="/data/chocula/villaa/PhotoN_SuperSim/sbberoot/";
   string dataset="";
   string source="sbbe";
   int numfiles=-1;
   int datasetno=0;
   bool evtgrp=true;

   for(int i=0; i<argc; i++)
   {
      rep=argv[i];
      if(rep =="-o")
        outfile = argv[i+1];
      if(rep =="-dir")
        outdir = argv[i+1];
      if(rep =="-indir")
        indir = argv[i+1];
      if(rep=="-f")
        numfiles = atoi(argv[i+1]);
      if(rep=="-E")
        evtgrp = true;
      if(rep=="-set")
        datasetno = atoi(argv[i+1]);
      if(rep=="-source")
        source = argv[i+1];
   }

   std::ostringstream oss;
   oss << std::hex << "0x" << std::setfill('0') << std::setw(4) << datasetno;
   dataset = oss.str(); 

   //get the data chain
   int nev; 
   TChain *ch = chainPhotoNSuperSim(nev,numfiles,datasetno,source,indir);


//make a file to save stuff in
//string savefile = "/data/chocula/villaa/PhotoN_SuperSim/sbbe_trees.root";
string savefile = outdir+"/"+outfile;
ifstream iofile(savefile.c_str());
TFile *f;
if(iofile){
  f = new TFile(savefile.c_str(),"UPDATE");
}
else{
  f = new TFile(savefile.c_str(),"RECREATE");
}
iofile.close();


for(int i=0;i<15;i++){
  cout << "Organizing data for zip " << i+1 << endl;
  TH1D *hs = new TH1D(Form("nrs_zip%d",i+1),Form("nrs_zip%d",i+1),100,0.0,5.0);
  TH1D *h = new TH1D(Form("nr_zip%d",i+1),Form("nr_zip%d",i+1),100,0.0,5.0);
  hs->SetLineColor(kRed);
  TTree *condensed = projectSumDepEv_AllInfo(ch,Form("allzips.nhits>0 && allzips.DetNum==%d",i+1),100000000,i+1);
  condensed->SetName(Form("sumzip%d",i+1));
  cout << "Filling histograms for zip " << i+1 << endl;
  cout << "Have " << condensed->GetEntries() << " entries in tree for zip " << i+1 << endl;
  //condensed->Draw(Form("edepNR/1000>>nrs_zip%d",i+1),"NRhit==1 && ERhit==0","goff");
  //condensed->Draw(Form("edepNR/1000>>nr_zip%d",i+1),"NRhit>1 && ERhit==0","goff");
  //condensed->Draw("edepNR","NRhit==1 && ERhit==0","goff");
  //condensed->Draw("edepNR","NRhit>1 && ERhit==0","goff");
 
  cout << "Writing TTree for zip " << i+1 << endl;
  condensed->Write(0,TObject::kOverwrite);
  //cout << "Writing h for zip " << i+1 << endl;
  //h->Write(0,TObject::kOverwrite);
  //cout << "Writing hs for zip " << i+1 << endl;
  //hs->Write(0,TObject::kOverwrite);
}
f->Close();
   cout << "Finished writing file. " << endl;
   return 0;
}
TTree *projectSumDepEv(TChain *ch,string cut,int maxev,int zip)
{
  Int_t nev;

  //total number of events
  Long64_t ntot = ch->GetEntries();
  cout << "Processing " << (Long64_t)ntot << " events" << endl;
  ch->SetEstimate(ntot);

  //write out the NR and ER cuts as strings
  string ERcut = "(allzips.PType==11 || allzips.PType==-11) || allzips.PType==22";
  string NRcut = "allzips.PType==2112 || allzips.PType-allzips.PType%10000>1";

  //make new TTree with total edepNR, edepER, nhit, NRhit, ERhit, positions?
  Long64_t nhit,NRhit,ERhit;
  Double_t edepNR,edepER;
   
  // create tree and the ROOT file to store it
  TTree *datatree = new TTree(Form("edeptree_zip%d",zip),"energy deposition data");
   
  // create branches in tree for each data column
  datatree->Branch("nhit",&nhit,"nhit/L");
  datatree->Branch("NRhit",&NRhit,"NRhit/L");
  datatree->Branch("ERhit",&ERhit,"ERhit/L");
  datatree->Branch("edepNR",&edepNR,"edepNR/D");
  datatree->Branch("edepER",&edepER,"edepER/D");

  //get meaningful quantities from the tree/chain
  string NRhitstr = "Sum$(1*(("+NRcut+") && ("+cut+")))";
  string ERhitstr = "Sum$(1*(("+ERcut+") && ("+cut+")))";
  string edepNRstr = "Sum$(allzips.Edep*(("+NRcut+") && ("+cut+")))";
  string edepERstr = "Sum$(allzips.Edep*(("+ERcut+") && ("+cut+")))";

  //do 100M at a time
  int iterations=ntot/maxev;

  if(ntot%maxev!=0) iterations++;

  for(int j=0;j<iterations;j++){
    cout << "iteration " << j+1 << " of " << iterations << endl;
    ch->Draw(Form("%s:%s:%s:%s",NRhitstr.c_str(),ERhitstr.c_str(),edepNRstr.c_str(),edepERstr.c_str()),Form("Entry$<%d && Entry$>=%d",(int)maxev*(j+1),(int)maxev*j),"goff");
    Long64_t nevents = ch->GetSelectedRows();
    Double_t *NRhitvec = ch->GetV1();
    Double_t *ERhitvec = ch->GetV2();
    Double_t *edepNRvec = ch->GetV3();
    Double_t *edepERvec = ch->GetV4();

    //loop through the lists and fill the tree;
    for(Long64_t i=0; i<nevents; i++){
      nhit=0;NRhit=0;ERhit=0;
      edepNR=0;edepER=0;

      NRhit=NRhitvec[i];
      ERhit=ERhitvec[i];
      edepNR=edepNRvec[i];
      edepER=edepERvec[i];
      nhit=NRhit+ERhit;
      datatree->Fill();
    }


  }

  cout << "Returning TTree for zip " << zip << endl;
  return datatree;
}
TTree *projectSumDepEv_AllInfo(TChain *ch,string cut,int maxev,int zip)
{
  Int_t nev;

  //total number of events
  Long64_t ntot = ch->GetEntries();
  cout << "Processing " << (Long64_t)ntot << " events" << endl;
  ch->SetEstimate(ntot);


  //make new TTree with total edepNR, edepER, nhit, NRhit, ERhit, positions?
  Long64_t nhit,NRhit,ERhit,totalevents;
  Double_t edepNR,edepER,edepNR_late,edepER_late;
  Double_t NRedep[10000],ERedep[10000],NRx[10000],NRy[10000],NRz[10000],NRYield[10000],NRt[10000],ERx[10000],ERy[10000],ERz[10000],ERYield[10000],ERt[10000],NRPType[10000],ERPType[10000];
   
  // create tree and the ROOT file to store it
  TTree *datatree = new TTree(Form("edeptree_zip%d",zip),"energy deposition data");
  
  totalevents = ntot;
  // create branches in tree for each data column
  datatree->Branch("totalevents",&totalevents,"totalevents/L");
  datatree->Branch("nhit",&nhit,"nhit/L");
  datatree->Branch("NRhit",&NRhit,"NRhit/L");
  datatree->Branch("ERhit",&ERhit,"ERhit/L");
  datatree->Branch("edepNR",&edepNR,"edepNR/D");
  datatree->Branch("edepER",&edepER,"edepER/D");
  datatree->Branch("edepNR_late",&edepNR_late,"edepNR/D");
  datatree->Branch("edepER_late",&edepER_late,"edepER/D");
  datatree->Branch("NRedep",&NRedep,"NRedep[NRhit]/D");
  datatree->Branch("NRPType",&NRPType,"NRPType[NRhit]/D");
  datatree->Branch("NRx",&NRx,"NRx[NRhit]/D");
  datatree->Branch("NRy",&NRy,"NRy[NRhit]/D");
  datatree->Branch("NRz",&NRz,"NRz[NRhit]/D");
  datatree->Branch("NRt",&NRt,"NRt[NRhit]/D");
  datatree->Branch("NRYield",&NRYield,"NRYield[NRhit]/D");
  datatree->Branch("ERedep",&ERedep,"ERedep[ERhit]/D");
  datatree->Branch("ERPType",&ERPType,"ERPType[ERhit]/D");
  datatree->Branch("ERx",&ERx,"ERx[ERhit]/D");
  datatree->Branch("ERy",&ERy,"ERy[ERhit]/D");
  datatree->Branch("ERz",&ERz,"ERz[ERhit]/D");
  datatree->Branch("ERt",&ERt,"ERt[NRhit]/D");
  datatree->Branch("ERYield",&ERYield,"ERYield[ERhit]/D");

  //we have to do this a little different, link to each event in chain by set branch address, 
  //then apply cut by TEventList, and use the method Next() to advance that list for each event passing
  //the cut.  Loop through GetN() events. Read the vectors in the chain for those, then fill the other tree

  //I think we need a max event here, use 10000?
  Int_t n;
  Double_t Edep[10000],PType[10000],X1[10000],Y1[10000],Z1[10000],Time1[10000],Yield[10000];

  //set the branch addresses
  ch->SetBranchAddress("allzips.nhits",&n);
  ch->SetBranchAddress("allzips.Edep",&Edep);
  ch->SetBranchAddress("allzips.PType",PType);
  ch->SetBranchAddress("allzips.X1",X1);
  ch->SetBranchAddress("allzips.Y1",Y1);
  ch->SetBranchAddress("allzips.Z1",Z1);
  ch->SetBranchAddress("allzips.Time1",Time1);
  ch->SetBranchAddress("allzips.Yield",Yield);

  //apply the cut to get nhits>0 and at least one of the recoil types I'm looking for
  TEventList *elist = new TEventList(Form("elist_it%d",zip));
  //TEventList *elistall = new TEventList(Form("elist_all%d",zip));
  ch->Draw(Form(">>elist_it%d",zip),cut.c_str(),"goff");
  //ch->Draw(Form(">>elist_all%d",zip),"","goff");
  Long64_t ncut = elist->GetN();
  ch->SetEventList(elist);
  //TEntryList *entrylist = ch->GetEntryList();

  ch->GetEvent(elist->GetEntry(0));
  //do 100M at a time
  int iterations=ncut/maxev;

  //cout << "Found " << ncut << " events matching the cut: " << cut << endl;

  if(ncut%maxev!=0) iterations++;

  int count=0;
  for(int j=0;j<iterations;j++){
    cout << "iteration " << j+1 << " of " << iterations << endl;

    for(int k=0;k<maxev;k++){
      if(count>=ncut) break;


      //set overall hits
      nhit=n;
      //cout << "Got event " << count << " with " << n << "hits" << endl;

      //intitalize aggregate variables
      NRhit=0;
      ERhit=0;
      edepNR=0.0;
      edepER=0.0;
      edepNR_late=0.0;
      edepER_late=0.0;

      if(n>10000){
	cerr << "Saturate!" << endl;
        count++;
	ch->GetEvent(elist->GetEntry(count));
	//write the event
	datatree->Fill();
	continue;
      }

  //write out the NR and ER cuts as strings
  //string ERcut = "(allzips.PType==11 || allzips.PType==-11) || allzips.PType==22";
  //string NRcut = "allzips.PType==2112 || allzips.PType-allzips.PType%10000>1";
      //loop through PType and fill appropriate variables
      for(int l=0;l<n;l++){
        if(PType[l]==11 || PType[l]==-11 || PType[l]==22){
	  ERhit++;
          edepER+=Edep[l];
	  if(Time1[l]>1e10) //ten seconds later
            edepER_late+=Edep[l];
	  ERedep[ERhit-1] = Edep[l];
	  ERPType[ERhit-1] = PType[l];
	  ERx[ERhit-1] = X1[l];
	  ERy[ERhit-1] = Y1[l];
	  ERz[ERhit-1] = Z1[l];
	  ERt[ERhit-1] = Time1[l];
	  ERYield[ERhit-1] = Yield[l];
	  //cout << "Filling ER for zip " << zip << ": " << Edep[l] << ", " << PType[l] << ", " << X1[l] << ", " << Y1[l] << ", " << Z1[l] << endl;
	}
	else if(PType[l]==2112 || ((Long64_t)PType-(Long64_t)PType%10000)>1){
	  NRhit++;
          edepNR+=Edep[l];
	  if(Time1[l]>1e10) //ten seconds later
            edepNR_late+=Edep[l];
	  NRedep[NRhit-1] = Edep[l];
	  NRPType[NRhit-1] = PType[l];
	  NRx[NRhit-1] = X1[l];
	  NRy[NRhit-1] = Y1[l];
	  NRz[NRhit-1] = Z1[l];
	  NRt[NRhit-1] = Time1[l];
	  NRYield[NRhit-1] = Yield[l];
	  //cout << "Filling NR for zip " << zip << ": " << Edep[l] << ", " << PType[l] << ", " << X1[l] << ", " << Y1[l] << ", " << Z1[l] << endl;
	}
      }

      //fill
      //cout << "Filling event number " << count << ", with total hits " << ERhit+NRhit << endl;
      datatree->Fill();
      //increment
      count++;
      //set the event
      ch->GetEvent(elist->GetEntry(count));
    }
  }

  //reset the event list for the tree
  ch->SetEventList(0);
  ch->GetEvent(0);
  cout << "Returning TTree for zip " << zip << endl;
  return datatree;
}
TChain *chainPhotoNSuperSim(int &nev,int n,int datasetno,string source,string dir,string treename)
{
   //get data set number string in hex
   string dataset="";
   std::ostringstream oss;
   oss << std::hex << "0x" << std::setfill('0') << std::setw(4) << datasetno;
   dataset = oss.str(); 

  //create a chain
  TChain *data = new TChain(treename.c_str(),treename.c_str());

  //make a command to use with a root pipe
  string command;
  command = "ls "+dir+" |grep .root |grep "+dataset+"_ |grep "+source+" |grep _merge";
  TString files = gSystem->GetFromPipe(command.c_str());
  std::istringstream iss(files.Data());

  //get the files from the string
  string filename;

  //make a counter
  int count=0;

  while(!iss.eof()){
    iss >> filename;
    if(n<0 || count<n){
      string fullfilename = dir+"/"+filename;
      data->Add(fullfilename.c_str());
    }
    count++;
  }
  
  return data;

}

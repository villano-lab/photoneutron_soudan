/*==================skimData_v2.C================================= 
   
      PROGRAMMER:  Anthony Villano 02/22/18

      UPDATES:      
       

      PURPOSE: The code in skimData_v2.C and skimData_v2.h is a replacement for
               the original skimming code to re-organize the events in the photoneutron
	       simulation.  This code currently is easiest to run inside
	       the ROOT interpreter.  It can be compiled for faster running.


	       The following members functions are called by the
	       TTree::Process functions.
	       
	       Begin():      called everytime a loop on the tree starts.
	                     a convenient place to create your histograms. 
			     
	       SlaveBegin():

	       Notify():     This function is called at the first entry
	                     of a new Tree in a chain.

	       Process():    called to analyze each entry.

	       SlaveTerminate():

	       Terminate():    called at the end of a loop on a TTree.  a
	                       convenient place to draw/fit your histograms.
              
======================================================================*/

#include "skimData_v2.h"
#include "TF1.h"
#include "TH1.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TLine.h"
#include "TMath.h"


//_____________________________________________________________________
void skimData_v2::Begin(TTree * /*tree*/)
{
// function called before starting the event loop
//  -it performs some cleanup
//  -it creates histograms
//  -it sets some initialisation for the entry list

   cout << "Got Into Begin" << endl;

  

   //print the option specified in the Process function.
   TString option = GetOption();
   Info("Begin", "starting skimData_v2 with process option: %s", option.Data());

   //process cases with entry list
   if (fChain) fChain->SetEntryList(0);
   delete gDirectory->GetList()->FindObject("elist");

   // case when one creates/fills the entry list
   if (option.Contains("fillList")) {
      fillList = kTRUE;
      // Add to the input list for processing in PROOF, if needed
      if (fInput) {
         fInput->Add(new TNamed("fillList",""));
         // We send a clone to avoid double deletes when importing the result
         fInput->Add(new TEntryList("elist", "GF capture selection from Cut"));
      }
   }
   // case when one uses the entry list generated in a previous call
   if (option.Contains("useList")) {
      useList  = kTRUE;
      if (fInput) {
         // Option "useList" not supported in PROOF directly
         Warning("Begin", "option 'useList' not supported in PROOF - ignoring");
         Warning("Begin", "the entry list must be set on the chain *before* calling Process");
      } else {
         TFile f("elist.root");
         elist = (TEntryList*)f.Get("elist");
         if (elist) elist->SetDirectory(0); //otherwise the file destructor will delete elist
      }
   }
}

//_____________________________________________________________________
void skimData_v2::SlaveBegin(TTree *tree)
{
// function called before starting the event loop
//  -it performs some cleanup
//  -it creates histograms
//  -it sets some initialisation for the entry list

   //initialize the Tree branch addresses
   Init(tree);

   //define some variables
   cout << "Got Into Slave Begin" << endl;

   //print the option specified in the Process function.
   TString option = GetOption();
   Info("SlaveBegin",
        "starting skimData_v2 with process option: %s (tree: %p)", option.Data(), tree);




   // Entry list stuff (re-parse option because on PROOF only SlaveBegin is called)
   if (option.Contains("fillList")) {
      fillList = kTRUE;
      // Get the list
      if (fInput) {
         if ((elist = (TEntryList *) fInput->FindObject("elist")))
            // Need to clone to avoid problems when destroying the selector
            elist = (TEntryList *) elist->Clone();
      }
      if (elist)
         fOutput->Add(elist);
      else
         fillList = kFALSE;
   }
}

//_____________________________________________________________________
Bool_t skimData_v2::Process(Long64_t entry)
{
// entry is the entry number in the current Tree
// Selection function to select D* and D0.

   chainentry = entry; //allow access across the class
   //in case one entry list is given in input, the selection has already been done.
   if (!useList) {
      //put cuts here
      Bool_t cutout;
      if (cutout) return kFALSE;
   }
   // if option fillList, fill the entry list
   if (fillList) elist->Enter(entry);

   // to read complete event, call fChain->GetTree()->GetEntry(entry)
   // read branches not processed in ProcessCut
   fChain->GetTree()->GetEntry(entry);
   //b_zip->GetEntry(entry);
   //b_cap->GetEntry(entry);
   //b_prim->GetEntry(entry);
   if(verbosity>0)
     arrPrint(zip_DetNum,zip_n);

   printf("in entry %5d of %7d\r",chaincount,elist->GetN());
   chaincount++;
   //clear out variables
   NRhit=0;
   ERhit=0;
   edepNR=0.0;
   edepER=0.0;
   edepNR_late=0.0;
   edepER_late=0.0;
   //clearvec(NRedep,10000);
   //clearvec(NRPType,10000);
   //clearvec(NRx,10000);
   //clearvec(NRy,10000);
   //clearvec(NRz,10000);
   //clearvec(NRt,10000);
   //clearvec(NRYield,10000);
   //clearvec(ERedep,10000);
   //clearvec(ERPType,10000);
   //clearvec(ERx,10000);
   //clearvec(ERy,10000);
   //clearvec(ERz,10000);
   //clearvec(ERt,10000);
   //clearvec(ERYield,10000);
   //clearvec(NRcapProj,10000);
   //clearvec(ERcapProj,10000);

   nhit=zip_n;

   //my arrays are only 10000 long, shouldn't have this man hits
   if(zip_n>10000 || cap_n>10000 || prim_n>10000){
     cerr << "Saturate!" << endl;
     ch->GetEvent(elist->GetEntry(count));
     //write the event
     datatree->Fill();
     return kFALSE;
   }

   //fill the capture data
   ncap=0;
   int nidx=0;
   for(int i=0;i<cap_n;i++){
     if(cap_PType[i]==2112 && cap_DetNum[i]==zip){ 
        ncap++; 
        Cx[nidx] = cap_X3[i];
        Cy[nidx] = cap_Y3[i];
        Cz[nidx] = cap_Z3[i];
        Ct[nidx] = cap_Time3[i];
        Cpx[nidx] = cap_Xmom3[i];
        Cpy[nidx] = cap_Ymom3[i];
        Cpz[nidx] = cap_Zmom3[i];
        CTS[nidx] = cap_TrkStep[i];
        CTrk[nidx] = (cap_TrkStep[i] - ((Long64_t)cap_TrkStep[i]%100000))/100000;
        nidx++;
     }
   }

   //go through captures and find the TrkStep of neutrons and times
   vector<Double_t> progeny_ts;
   vector<Double_t> progeny_trk;
   for(int i=0;i<ncap;i++){
     progeny_ts.push_back(CTS[i]);
     progeny_trk.push_back(CTrk[i]);
   }
   //now you gotta iteratively go through the zip list to find things in the progeny
   for(int i=0;i<zip_n;i++) zip_InCapProg[i] = false; //start assuming NOT in progeny
   Bool_t foundnew=true;
   while(foundnew){
     foundnew=false;
     for(int i=0;i<zip_n;i++){
       if(zip_DetNum[i]==zip && isInVector(zip_Parent[i],progeny_trk) && !isInVector(zip_TrkStep[i],progeny_ts)){
         foundnew=true;
	 //add to progeny
	 progeny_ts.push_back(zip_TrkStep[i]);
	 progeny_trk.push_back((zip_TrkStep[i]-(Long64_t)zip_TrkStep[i]%100000)/100000);
	 zip_InCapProg[i] = true;
       }
     }
   }
   //cout << "Finished progeny for entry: " << entry << endl;
   //cout << "found " << progeny_ts.size() << " hits in progeny" << endl;

   //do stuff to this zip data
    for(int l=0;l<zip_n;l++){
      if(zip_DetNum[l]!=zip) continue;
      if((zip_PType[l]==11 || zip_PType[l]==-11) || zip_PType[l]==22){
        ERhit++;
	if(!zip_InCapProg[l])
          edepER+=zip_Edep[l];
        if(!zip_InCapProg[l] && zip_Time1[l]>1e10) //ten seconds later
          edepER_late+=zip_Edep[l];
        ERedep[ERhit-1] = zip_Edep[l];
        ERPType[ERhit-1] = zip_PType[l];
        ERx[ERhit-1] = zip_X1[l];
        ERy[ERhit-1] = zip_Y1[l];
        ERz[ERhit-1] = zip_Z1[l];
        ERt[ERhit-1] = zip_Time1[l];
        ERYield[ERhit-1] = zip_Yield[l];
	ERcapProg[ERhit-1] = (Double_t)zip_InCapProg[l];
      }
      else if(zip_PType[l]==2112 || ((Long64_t)zip_PType-(Long64_t)zip_PType%10000)>1){
        NRhit++;
	if(!zip_InCapProg[l])
          edepNR+=zip_Edep[l];
	if(!zip_InCapProg[l] && zip_Time1[l]>1e10) //ten seconds later
          edepNR_late+=zip_Edep[l];
	NRedep[NRhit-1] = zip_Edep[l];
	NRPType[NRhit-1] = zip_PType[l];
	NRx[NRhit-1] = zip_X1[l];
	NRy[NRhit-1] = zip_Y1[l];
	NRz[NRhit-1] = zip_Z1[l];
	NRt[NRhit-1] = zip_Time1[l];
	NRYield[NRhit-1] = zip_Yield[l];
	NRcapProg[NRhit-1] = (Double_t)zip_InCapProg[l];
      }
    }


    //fill the primary data
    //NOTE: the primary data is just taken over as is 
    //by linking the new
    //tree to the already-existing data structures
    //nprim=prim_n;
    ////cout << nprim << endl;
    //for(int i=0;i<nprim;i++){
    //  //cout << "idx: " << i << endl;
    //  //cout << "prim type: " << prim_PType[i] << endl;
    //  //cout << "prim KE: " << prim_KE[i] << endl;
    //  PPType[i] = prim_PType[i];
    //  PKE[i] = prim_KE[i];
    //  PX[i] = prim_X[i];
    //  PY[i] = prim_Y[i];
    //  PZ[i] = prim_Z[i];
    //  PXmom[i] = prim_Xmom[i];
    //  PYmom[i] = prim_Ymom[i];
    //  PZmom[i] = prim_Zmom[i];
    //}

    //fill
    fOutTree->Fill();

   return kTRUE;
}


//_____________________________________________________________________
void skimData_v2::SlaveTerminate()
{
   // nothing to be done
}

//_____________________________________________________________________
void skimData_v2::Terminate()
{
// function called at the end of the event loop

   //save the entry list to a Root file if one was produced
   if (fillList) {
      elist = dynamic_cast<TEntryList*>(fOutput->FindObject("elist"));
      if (elist) {
         TFile efile("elist.root","recreate");
         elist->Write();
      } else {
         Error("Terminate", "entry list requested but not found in output");
      }
   }
}
//_____________________________________________________________________
void skimData_v2::SetConfiguration(Double_t in_detlength, Double_t in_density, Int_t in_partcount,
string in_matname,
string in_munuc,
string in_part,
string in_energy)
{

  return;
}
//_____________________________________________________________________
void skimData_v2::arrPrint(Double_t *arr,Int_t n)
{
  cout << "Array for fChain event: " << chainentry << "=============" << endl;
  for(int i=0;i<n;i++){
    cout << arr[i] << endl;
  }
  cout << "==================================================================" << endl;
  return;
}
//_____________________________________________________________________
void skimData_v2::clearvec(double*vec,int size)
{
  for(int i=0;i<size;i++) vec[i]=-1;	
}
//_____________________________________________________________________
Bool_t skimData_v2::isInVector(Double_t element,vector<Double_t> list)
{
  for(int i=0;i<list.size();i++) {
    if(element==list[i]) return true;	
  }
  return false;
}

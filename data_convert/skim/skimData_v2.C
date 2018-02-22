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
void skimdata_v2::SlaveBegin(TTree *tree)
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
   fChain->GetTree()->GetEntry(entry)
   //b_zip->GetEntry(entry);
   //b_cap->GetEntry(entry);
   //b_prim->GetEntry(entry);


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
void skimData_v2::arrPrint(Double_t *arr,Int_t n);
{
  cout << "Array for fChain event: " << chainentry << "=============" << endl;
  for(int i=0;i<n;i++){
    cout << arr[i] << endl;
  }
  cout << "==================================================================" << endl;
  return;
}

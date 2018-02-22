/*==================skimData_v2.h================================= 
   
      PROGRAMMER:  Anthony Villano 02/22/18

      UPDATES:      
       

      PURPOSE: The header file for the skimData_v2 class which will
               work with TTree::Process function.  This class is a
	       specific realization of a TSelector class see the header
	       of the .C file for more information.
              
======================================================================*/


#ifndef skimData_v2_h
#define skimData_v2_h

//#include<map>
//#include<vector>
#include <climits> //LONG_MAX def

#include <TROOT.h>
#include <TChain.h>
#include <TEntryList.h>
#include <TFile.h>
#include <TSelector.h>

class   TH1D;

class skimData_v2 : public TSelector {
   public :

   Long64_t        chainentry;
   Int_t           verbosity;
   Bool_t          useList;
   Bool_t          fillList;
   TEntryList     *elist;


   TTree          *fChain;    //pointer to the analyzed TTree or TChain
   //Declaration of leaves types
   //Zip stuff
   Long64_t        zip_n;
   Double_t        zip_Edep[10000];
   Double_t        zip_PType[10000];
   Double_t        zip_KE[10000];
   Double_t        zip_X1[10000];
   Double_t        zip_Y1[10000];
   Double_t        zip_Z1[10000];
   Double_t        zip_Time1[10000];
   Double_t        zip_Xmom1[10000];
   Double_t        zip_Ymom1[10000];
   Double_t        zip_Zmom1[10000];
   Double_t        zip_Yield[10000];
   Double_t        zip_DetNum[10000];

   //capture stuff
   Long64_t        cap_n;
   Double_t        cap_PType[10000];
   Double_t        cap_KE[10000];
   Double_t        cap_X3[10000];
   Double_t        cap_Y3[10000];
   Double_t        cap_Z3[10000];
   Double_t        cap_Xmom3[10000];
   Double_t        cap_Ymom3[10000];
   Double_t        cap_Zmom3[10000];
   Double_t        cap_Time3[10000];
   Double_t        cap_DetNum[10000];

   //primaries stuff
   Long64_t        prim_n;
   Double_t        prim_PType[10000];
   Double_t        prim_KE[10000];
   Double_t        prim_X[10000];
   Double_t        prim_Y[10000];
   Double_t        prim_Z[10000];
   Double_t        prim_Xmom[10000];
   Double_t        prim_Ymom[10000];
   Double_t        prim_Zmom[10000];

   //List of branches
   TBranch        *b_zip;
   TBranch        *b_cap;
   TBranch        *b_prim;

   skimData_v2(TTree *tree=0);
   ~skimData_v2();
   int     Version() const {return 1;}
   void    Begin(TTree *tree);
   void    SlaveBegin(TTree *tree);
   void    Init(TTree *tree);
   Bool_t  Notify();
   Bool_t  Process(Long64_t entry);
   void    SetOption(const char *option) { fOption = option; }
   void    SetObject(TObject *obj) { fObject = obj; }
   void    SetVerbosity(Int_t v){verbosity=v;}
   //void    SetInputList(TList *input) {fInput = input;}
   //TList  *GetOutputList() const { return fOutput; }
   void    SlaveTerminate();
   void    Terminate();

   //some functions for setting/getting private stuff
   void arrPrint(Double_t *arr,Int_t n);
   void SetConfiguration(Double_t in_detlength, Double_t in_density, Int_t in_partcount,
                  string in_matname="myScintC9H12",
                  string in_munuc="on",
                  string in_part="mu",
                  string in_energy="280");

   ClassDef(skimData_v2,0);

   private :

   //functions for specialized procedures 
   //void resetThermTracks();
};

#endif


//_____________________________________________________________________
skimData_v2::skimData_v2(TTree * /*tree*/)
{
// Build the chain of Root files
//
   cout << "got to the constructor" << endl;
   elist = 0;
   fChain = 0;
   verbosity=0;
   fillList = kFALSE;
   useList  = kFALSE;

   //set default values for things

}

//_____________________________________________________________________
skimData_v2::~skimData_v2()
{
}

//_____________________________________________________________________
void skimData_v2::Init(TTree *tree)
{
   //   Set branch addresses

   cout << "got to the Init" << endl;
   Info("Init","tree: %p", tree);

   if (tree == 0) return;
   fChain    = tree;
   //zip stuff
   fChain->SetBranchAddress("allzips.nhits",&zip_n,&(b_zip));
   fChain->SetBranchAddress("allzips.Edep",&zip_Edep,&(b_zip));
   fChain->SetBranchAddress("allzips.PType",&zip_PType,&(b_zip));
   fChain->SetBranchAddress("allzips.KE",&zip_KE,&(b_zip));
   fChain->SetBranchAddress("allzips.X1",&zip_X1,&(b_zip));
   fChain->SetBranchAddress("allzips.Y1",&zip_Y1,&(b_zip));
   fChain->SetBranchAddress("allzips.Z1",&zip_Z1,&(b_zip));
   fChain->SetBranchAddress("allzips.Time1",&zip_Time1,&(b_zip));
   fChain->SetBranchAddress("allzips.Xmom1",&zip_Xmom1,&(b_zip));
   fChain->SetBranchAddress("allzips.Ymom1",&zip_Ymom1,&(b_zip));
   fChain->SetBranchAddress("allzips.Zmom1",&zip_Zmom1,&(b_zip));
   fChain->SetBranchAddress("allzips.Yield",&zip_Yield,&(b_zip));
   fChain->SetBranchAddress("allzips.DetNum",&zip_DetNum,&(b_zip));

   //capture stuff
   fChain->SetBranchAddress("mcnCapture.nhits",&cap_n,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.PType",&cap_PType,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.KE",&cap_KE,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.X3",&cap_X3,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.Y3",&cap_Y3,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.Z3",&cap_Z3,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.Time3",&cap_Time3,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.Xmom3",&cap_Xmom3,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.Ymom3",&cap_Ymom3,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.Zmom3",&cap_Zmom3,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.DetNum",&cap_DetNum,&(b_cap));

   //primaries stuff
   fChain->SetBranchAddress("mcprimary.nprimaries",&prim_n,&(b_prim));
   fChain->SetBranchAddress("mcprimary.PType",&prim_PType,&(b_prim));
   fChain->SetBranchAddress("mcprimary.KE",&prim_KE,&(b_prim));
   fChain->SetBranchAddress("mcprimary.X",&prim_X,&(b_prim));
   fChain->SetBranchAddress("mcprimary.Y",&prim_Y,&(b_prim));
   fChain->SetBranchAddress("mcprimary.Z",&prim_Z,&(b_prim));
   fChain->SetBranchAddress("mcprimary.Xmom",&prim_Xmom,&(b_prim));
   fChain->SetBranchAddress("mcprimary.Ymom",&prim_Ymom,&(b_prim));
   fChain->SetBranchAddress("mcprimary.Zmom",&prim_Zmom,&(b_prim));
}

//_____________________________________________________________________
Bool_t skimData_v2::Notify()
{
//   called when loading a new file
//   get branch pointers

   Info("Notify","processing file: %s",fChain->GetCurrentFile()->GetName());

   if (elist && fChain) {
      if (fillList) {
         elist->SetTree(fChain);
      } else if (useList) {
         fChain->SetEntryList(elist);
      }
   }
   return kTRUE;
}

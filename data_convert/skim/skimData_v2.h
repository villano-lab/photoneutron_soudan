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
#include <map>

#include <TROOT.h>
#include <TChain.h>
#include <TEntryList.h>
#include <TEventList.h>
#include <TFile.h>
#include <TSelector.h>

class   TH1D;
class   TH3D;

class skimData_v2 : public TSelector {
   public :

   Long64_t        chainentry;
   Long64_t        chaincount;
   Int_t           verbosity;
   Bool_t          useList;
   Bool_t          fillList;
   TEntryList     *elist;


   TTree          *fChain;    //pointer to the analyzed TTree or TChain
   TTree          *fSimData;    //pointer to the analyzed TTree or TChain
   TTree          *fOutTree;    //pointer to the Output TTree or TChain
   TH3D           *vmap_70_0; //pointer to the voltage map
   Int_t          zip; //zip to do the tree for
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
   Double_t        zip_TrkStep[10000];
   Double_t        zip_Parent[10000];
   Bool_t          zip_InCapProg[10000]; //not linked to tree

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
   Double_t        cap_TrkStep[10000];
   Double_t        cap_Parent[10000];

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

   //vars for new TTree
   Long64_t     nhit;
   Long64_t     NRhit;
   Long64_t     ERhit;
   Long64_t     totalevents;
   Long64_t     eventnum;
   Double_t     edepNR;
   Double_t     edepER;
   Double_t     edepNR_late;
   Double_t     edepER_late;
   Double_t     NRedep[10000];
   Double_t     ERedep[10000];
   Double_t     NRx[10000];
   Double_t     NRy[10000];
   Double_t     NRz[10000];
   Double_t     NRYield[10000];
   Double_t     NRt[10000];
   Double_t     ERx[10000];
   Double_t     ERy[10000];
   Double_t     ERz[10000];
   Double_t     ERYield[10000];
   Double_t     ERt[10000];
   Double_t     NRPType[10000];
   Double_t     ERPType[10000];
   Double_t     NRcapProg[10000]; //this has NRhit entries
   Double_t     ERcapProg[10000]; //this has ERhit entries
   Long64_t     ncap;
   Double_t     Cx[10000];  //next three have ncap entries
   Double_t     Cy[10000];
   Double_t     Cz[10000];
   Double_t     Ct[10000];
   Double_t     CKE[10000];
   Double_t     Cpx[10000];  //next two have ncap entries
   Double_t     Cpy[10000];
   Double_t     Cpz[10000];
   Double_t     CTS[10000];  // these two are NOT written to file
   Double_t     CTrk[10000];
   Long64_t     nprim;
   Double_t     PPType[10000];
   Double_t     PKE[10000];
   Double_t     PX[10000];
   Double_t     PY[10000];
   Double_t     PZ[10000];
   Double_t     PXmom[10000];
   Double_t     PYmom[10000];
   Double_t     PZmom[10000];
   
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
   void    SetZip(Int_t z){zip=z;}
   void    SetSimDataChain(TTree *simdata);
   void    SetVoltageMaps(TH3D *mapweight,TH3D *mapcount,TString label);
   TTree*  GetOutTree(){return fOutTree;}
   //void    SetInputList(TList *input) {fInput = input;}
   //TList  *GetOutputList() const { return fOutput; }
   void    SlaveTerminate();
   void    Terminate();

   //some functions for setting/getting private stuff
   void arrPrint(Double_t *arr,Int_t n);
   Bool_t isInVector(Double_t element,vector<Double_t> list);
   void SetConfiguration(Double_t in_detlength, Double_t in_density, Int_t in_partcount,
                  string in_matname="myScintC9H12",
                  string in_munuc="on",
                  string in_part="mu",
                  string in_energy="280");

   ClassDef(skimData_v2,0);

   private :

   //functions for specialized procedures 
   void clearvec(double*,int);
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
   chaincount=0;
   zip = 14; //for now
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
   fChain->SetBranchAddress("allzips.TrkStep",&zip_TrkStep,&(b_zip));
   fChain->SetBranchAddress("allzips.Parent",&zip_Parent,&(b_zip));

   //capture stuff
   fChain->SetBranchAddress("mcnCapture.nhits",&cap_n,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.PType",&cap_PType,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.KE",&cap_KE,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.X3",&cap_X3,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.Y3",&cap_Y3,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.Z3",&cap_Z3,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.Time3",&cap_Time3,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.Xmom1",&cap_Xmom3,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.Ymom1",&cap_Ymom3,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.Zmom1",&cap_Zmom3,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.DetNum",&cap_DetNum,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.TrkStep",&cap_TrkStep,&(b_cap));
   fChain->SetBranchAddress("mcnCapture.Parent",&cap_Parent,&(b_cap));

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

   //hook up the output tree to the variables
   fOutTree = new TTree(Form("edeptree_zip%d",zip),"energy deposition data");
   fOutTree->Branch("eventnum",&eventnum,"eventnum/L");
   fOutTree->Branch("totalevents",&totalevents,"totalevents/L");
   fOutTree->Branch("nhit",&nhit,"nhit/L");
   fOutTree->Branch("NRhit",&NRhit,"NRhit/L");
   fOutTree->Branch("ERhit",&ERhit,"ERhit/L");
   fOutTree->Branch("edepNR",&edepNR,"edepNR/D");
   fOutTree->Branch("edepER",&edepER,"edepER/D");
   fOutTree->Branch("edepNR_late",&edepNR_late,"edepNR/D");
   fOutTree->Branch("edepER_late",&edepER_late,"edepER/D");
   fOutTree->Branch("NRedep",&NRedep,"NRedep[NRhit]/D");
   fOutTree->Branch("NRPType",&NRPType,"NRPType[NRhit]/D");
   fOutTree->Branch("NRx",&NRx,"NRx[NRhit]/D");
   fOutTree->Branch("NRy",&NRy,"NRy[NRhit]/D");
   fOutTree->Branch("NRz",&NRz,"NRz[NRhit]/D");
   fOutTree->Branch("NRt",&NRt,"NRt[NRhit]/D");
   fOutTree->Branch("NRYield",&NRYield,"NRYield[NRhit]/D");
   fOutTree->Branch("ERedep",&ERedep,"ERedep[ERhit]/D");
   fOutTree->Branch("ERPType",&ERPType,"ERPType[ERhit]/D");
   fOutTree->Branch("ERx",&ERx,"ERx[ERhit]/D");
   fOutTree->Branch("ERy",&ERy,"ERy[ERhit]/D");
   fOutTree->Branch("ERz",&ERz,"ERz[ERhit]/D");
   fOutTree->Branch("ERt",&ERt,"ERt[ERhit]/D");
   fOutTree->Branch("ERYield",&ERYield,"ERYield[ERhit]/D");
   fOutTree->Branch("NRcapProg",&NRcapProg,"NRcapProg[NRhit]/D");
   fOutTree->Branch("ERcapProg",&ERcapProg,"ERcapProg[ERhit]/D");
   fOutTree->Branch("nprim",&prim_n,"nprim/L");
   fOutTree->Branch("prim_PType",&prim_PType,"prim_PType[nprim]/D");
   fOutTree->Branch("prim_KE",&prim_KE,"prim_KE[nprim]/D");
   fOutTree->Branch("prim_X",&prim_X,"prim_X[nprim]/D");
   fOutTree->Branch("prim_Y",&prim_Y,"prim_Y[nprim]/D");
   fOutTree->Branch("prim_Z",&prim_Z,"prim_Z[nprim]/D");
   fOutTree->Branch("prim_Xmom",&prim_Xmom,"prim_Xmom[nprim]/D");
   fOutTree->Branch("prim_Ymom",&prim_Ymom,"prim_Ymom[nprim]/D");
   fOutTree->Branch("prim_Zmom",&prim_Zmom,"prim_Zmom[nprim]/D");
   fOutTree->Branch("ncap",&ncap,"ncap/L");
   fOutTree->Branch("cap_KE",&CKE,"cap_KE[ncap]/D");
   fOutTree->Branch("cap_X",&Cx,"cap_X[ncap]/D");
   fOutTree->Branch("cap_Y",&Cy,"cap_Y[ncap]/D");
   fOutTree->Branch("cap_Z",&Cz,"cap_Z[ncap]/D");
   fOutTree->Branch("cap_Time",&Ct,"cap_Time[ncap]/D");
   fOutTree->Branch("cap_Xmom",&Cpx,"cap_Xmom[ncap]/D");
   fOutTree->Branch("cap_Ymom",&Cpy,"cap_Ymom[ncap]/D");
   fOutTree->Branch("cap_Zmom",&Cpz,"cap_Zmom[ncap]/D");

   //apply event list to cut on ZIP and nhits>0
   cout << "Cutting to get only zip " <<  zip << " events" << endl;
   ostringstream cutstream;
   //cutstream << "allzips.DetNum==" << zip << " && allzips.nhits>0 ";
   cutstream << "allzips.nhits>0";
   cut = cutstream.str();
   cout << cut << endl;
   //string cut = "allzips.DetNum==14";
   elist = new TEntryList(Form("elist_it%d",zip),Form("elist_it%d",zip));
   Long64_t ncut;
   fChain->Draw(Form(">>elist_it%d",zip),cut.c_str(),"entrylist");
   ncut = elist->GetN();
   cout << "Selected " << ncut << " events out of " << fChain->GetEntries() << " total." << endl;
   fChain->SetEntryList(elist);


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
//_____________________________________________________________________
void skimData_v2::SetSimDataChain(TTree *simdata){
  fSimData = simdata;
   //set total number of primaries
   if(fSimData){
     totalevents=0;
     Double_t* vecprim;
     fSimData->Draw("Events","","goff");
     Int_t nch = fSimData->GetSelectedRows();
     vecprim = fSimData->GetV1();
     for(int i=0;i<nch;i++)
       totalevents+=vecprim[i];
   }
   return;
}
//_____________________________________________________________________
void skimData_v2::SetVoltageMaps(TH3D *mapweight, TH3D *mapcount, TString label){

  //store the resulting map in global
  if(label=="70V_0extrap"){
     vmap_70_0 = (TH3D*)mapweight->Clone();
  }
  else
    return;

  int nbinsX3D = mapweight->GetNbinsX();
  int nbinsY3D = mapweight->GetNbinsY();
  int nbinsZ3D = mapweight->GetNbinsZ();
  int nbins3D = (nbinsX3D+2)*(nbinsY3D+2)*(nbinsZ3D+2); //plus 2 is for overflow and underflow bins

  for(int binItr3D = 1; binItr3D <= nbins3D; binItr3D++)
  {
    double weightedBinContent3D = vmap_70_0->GetBinContent(binItr3D);
    double nSamples3D = mapcount->GetBinContent(binItr3D);
    double mapValue3D = (nSamples3D > 0 ? (weightedBinContent3D/nSamples3D) : 0);

    if(nSamples3D > 1)
      cout <<"nSamples3D = " << nSamples3D << endl;

    //now overwrite the bin content 
    vmap_70_0->SetBinContent(binItr3D, mapValue3D);
  }
  
  return;
}

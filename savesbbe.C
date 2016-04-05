#include <stdlib.h>
#include "treeUtil.C"
#include "chainPhotoNSuperSim.C"

void savesbbe(int zip=14)
{
gErrorIgnoreLevel = kFatal;

int nev;
TChain *ch = chainPhotoNSuperSim(nev,-1,2,"sbbe","/data/chocula/villaa/PhotoN_SuperSim/sbberoot/");

//make a file to save stuff in
string savefile = "/data/chocula/villaa/PhotoN_SuperSim/sbbe_trees.root";
ifstream iofile(savefile.c_str());
TFile *f;
if(iofile){
  f = new TFile(savefile.c_str(),"UPDATE");
}
else{
  f = new TFile(savefile.c_str(),"RECREATE");
}
iofile.close();


for(int i=zip-1;i<zip;i++){
  cout << "Organizing data for zip " << i+1 << endl;
  TH1D *hs = new TH1D(Form("nrs_zip%d",i+1),Form("nrs_zip%d",i+1),100,0.0,5.0);
  TH1D *h = new TH1D(Form("nr_zip%d",i+1),Form("nr_zip%d",i+1),100,0.0,5.0);
  hs->SetLineColor(kRed);
  TTree *condensed = projectSumDepEv(ch,Form("allzips.DetNum==%d",i+1),100000000);
  condensed->SetName(Form("sumzip%d",i+1));
  condensed->Draw(Form("edepNR/1000>>nrs_zip%d",i+1),"NRhit==1 && ERhit==0","");
  condensed->Draw(Form("edepNR/1000>>nr_zip%d",i+1),"NRhit>1 && ERhit==0","");
  
  TCanvas *c1 = new TCanvas;
  c1->SetLogy();
  h->Draw();
  hs->Draw("same");
  condensed->Write(0,TObject::kOverwrite);
  h->Write(0,TObject::kOverwrite);
  hs->Write(0,TObject::kOverwrite);
}
f->Close();


return; 
}

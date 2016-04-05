#include <stdlib.h>
#include "treeUtil.C"
#include "chainPhotoNSuperSim.C"

void plotybe()
{
gErrorIgnoreLevel = kFatal;

int nev;
TH1D *hs = new TH1D("nrs","nrs",100,0.0,20.0);
TH1D *h = new TH1D("nr","nr",100,0.0,20.0);
hs->SetLineColor(kRed);
TChain *ch = chainPhotoNSuperSim(nev,-1,2,"ybe","/data/chocula/villaa/PhotoN_SuperSim/yberoot/");
TTree *condensed_4 = projectSumDepEv(ch,"allzips.DetNum==4",100000000);
condensed_4->Draw("edepNR/1000>>nrs","NRhit==1 && ERhit==0","");
condensed_4->Draw("edepNR/1000>>nr","NRhit>1 && ERhit==0","");

TCanvas *c1 = new TCanvas;
c1->SetLogy();
h->Draw();
hs->Draw("same");


return; 
}
Double_t g2(Double_t *x, Double_t *par) {
   Double_t r1 = Double_t((x[0]-par[1])/par[2]);
   Double_t r2 = Double_t((x[1]-par[3])/par[4]);
   return par[0]*TMath::Exp(-0.5*(r1*r1+r2*r2));
}   

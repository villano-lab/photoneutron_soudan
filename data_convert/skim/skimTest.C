#include "../../root/chainPhotoNSuperSim.C"
#include "skimData_v2.C"
void skimTest(int zip=14, string file="/data/chocula/villaa/PhotoN_SuperSim/yberoot/ybe_SetID0x0004_p1000000_180221_160626_046866297_10180221_0000_merged.root"
){
cout << "here" << endl;
Int_t nev;
TChain *ch = chainPhotoNSuperSim(nev,-1,4,"ybe","/data/chocula/villaa/PhotoN_SuperSim/yberoot","mcmerged");
//TChain *ch = chainPhotoNSuperSim(nev,-1,4,"ybe");
gErrorIgnoreLevel = kFatal;
//TFile *f = new TFile(file.c_str());
//TTree *t = (TTree*) f->Get("mcmerged");
skimData_v2 *test = new skimData_v2();
////test->SetConfiguration(length,density,partcount);
test->SetZip(zip);
//test->SetVerbosity(1);
//TChain *ch = (TChain*) t; //recast
ch->Process(test);
TFile *fout = new TFile("out.root","RECREATE");
//TFile *f= new TFile(Form("/afs/slac.stanford.edu/u/dm/%s/geantfluka/GeantFluka_Data/GFCapData_%s_on_%s_%s.root",user.c_str(), material.c_str(), part.c_str(), energy.c_str()), "recreate");
//list->Write();
TTree *outtree = test->GetOutTree();
outtree->Write(0,TObject::kOverwrite);
fout->Close();
//f->Close();
}

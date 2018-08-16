#include "../../root/chainPhotoNSuperSim.C"
#include "skimData_v2.C"
#include "time.h"
void skimTest_wVoltage(int zip=14, string type="ybe", Int_t maxfiles=-1, int dataset=4, string suffix="1-3B"
){

ostringstream setID;
setID << "0x" << setw(4) << setfill('0') << hex << dataset;
time_t rawtime;
struct tm * timeinfo;
char date[7]; //zero termination

time (&rawtime);
timeinfo = localtime (&rawtime);

strftime (date,7,"%y%m%d",timeinfo);
cout << setID.str() << endl;
cout << type << endl;
cout << date << endl;
cout << Form("/data/chocula/villaa/PhotoN_SuperSim/ZipSum/%s_z%d_%s_%s_%s.root",type.c_str(),zip,date,setID.str().c_str(),suffix.c_str()) << endl;
Int_t nev;
TChain *ch = chainPhotoNSuperSim(nev,maxfiles,dataset,Form("%s",type.c_str()),Form("/data/chocula/villaa/PhotoN_SuperSim/%sroot",type.c_str()),"mcmerged");
TChain *infoch = chainPhotoNSuperSim(nev,maxfiles,dataset,Form("%s",type.c_str()),Form("/data/chocula/villaa/PhotoN_SuperSim/%sroot",type.c_str()),"G4SettingsInfoDir/runtime");
//TChain *ch = chainPhotoNSuperSim(nev,-1,4,"ybe");
gErrorIgnoreLevel = kFatal;
//TFile *f = new TFile(file.c_str());
//TTree *t = (TTree*) f->Get("mcmerged");
skimData_v2 *test = new skimData_v2();
////test->SetConfiguration(length,density,partcount);
test->SetZip(zip);
test->SetSimDataChain(infoch);

TFile *fvmaps = new TFile("voltage_map/voltagemap_0extrap.root");
TH3D *voltageMapWeighted = fvmaps->Get("voltageMapWeighted");
TH3D *voltageMapCount = fvmaps->Get("voltageMapCount");
test->SetVoltageMaps(voltageMapWeighted,voltageMapCount,"70V_0ex");
//test->SetVerbosity(1);
//TChain *ch = (TChain*) t; //recast
ch->Process(test);
TFile *fout = new TFile(Form("/data/chocula/villaa/PhotoN_SuperSim/ZipSum/%s_z%d_%s_%s_%s.root",type.c_str(),zip,date,setID.str().c_str(),suffix.c_str()),"RECREATE");
//TFile *fout = new TFile(Form("/data/chocula/villaa/PhotoN_SuperSim/ZipSum/ybe_z%d_180223_0x0004_100M.root",zip),"RECREATE");
//TFile *fout = new TFile("out.root","RECREATE");
//TFile *f= new TFile(Form("/afs/slac.stanford.edu/u/dm/%s/geantfluka/GeantFluka_Data/GFCapData_%s_on_%s_%s.root",user.c_str(), material.c_str(), part.c_str(), energy.c_str()), "recreate");
//list->Write();
TTree *outtree = test->GetOutTree()->Clone();
fout->cd();
outtree->Scan("NRx:NRy:NRz:NR_V70_0","","");
outtree->Write(0,TObject::kOverwrite);
fout->Close();
//f->Close();
}

#include "physicalFunctions.C"

TH1D *findNREvents_forDMC_Prompt(int zip,int maxev=10000, string file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/sbbe_trees.root",string outfile="dmc.txt")
{
  //delay time 3.2 ms
  double sec = 3.2e-3; //seconds
  bool delayed = false;

  //hard-code the cuts for now; lower endpoint inclusive
  int NRhitsMin = 1;
  int NRhitsMax = 10000000; //essentially infinity
  int ERhitsMin = 0;
  int ERhitsMax = 1;
  double TotalEMin = 200; //eV
  double TotalEMax = 1000000000; //essentially infinity


  bool delayed = true;
  if(sec<0.0){
    cout << "Delayed !" << endl;
    sec = TMath::Abs(sec);
    delayed = true;
  }

  //open the file and get the tree
  cout << "Opening file: " << file << endl;
  TFile *f = new TFile(file.c_str());
  if(!f){
    cout << "findNREvents_forDMC_Prompt: ERROR! could not read file" << endl;
    return NULL;
  }
  TTree *datatree = f->Get(Form("sumzip%d",zip));

  //create a histogram
  TH1D *h = new TH1D(Form("dmc_zip%d",zip),Form("dmc_zip%d",zip),300,TotalEMin/1000.0,8000/1000.0);

  //link to the branches
  Long64_t nhit,NRhit,ERhit,totalevents;
  Double_t edepNR,edepER,edepNR_late,edepER_late;
  Double_t NRedep[10000],ERedep[10000],NRx[10000],NRy[10000],NRz[10000],NRYield[10000],NRt[10000],ERx[10000],ERy[10000],ERz[10000],ERYield[10000],ERt[10000],NRPType[10000],ERPType[10000];
   
  // create branches in tree for each data column
  datatree->SetBranchAddress("totalevents",&totalevents);
  datatree->SetBranchAddress("nhit",&nhit);
  datatree->SetBranchAddress("NRhit",&NRhit);
  datatree->SetBranchAddress("ERhit",&ERhit);
  datatree->SetBranchAddress("edepNR",&edepNR);
  datatree->SetBranchAddress("edepER",&edepER);
  datatree->SetBranchAddress("edepNR_late",&edepNR_late);
  datatree->SetBranchAddress("edepER_late",&edepER_late);
  datatree->SetBranchAddress("NRedep",NRedep);
  datatree->SetBranchAddress("NRPType",NRPType);
  datatree->SetBranchAddress("NRx",NRx);
  datatree->SetBranchAddress("NRy",NRy);
  datatree->SetBranchAddress("NRz",NRz);
  datatree->SetBranchAddress("NRt",NRt);
  datatree->SetBranchAddress("NRYield",NRYield);
  datatree->SetBranchAddress("ERedep",ERedep);
  datatree->SetBranchAddress("ERPType",ERPType);
  datatree->SetBranchAddress("ERx",ERx);
  datatree->SetBranchAddress("ERy",ERy);
  datatree->SetBranchAddress("ERz",ERz);
  datatree->SetBranchAddress("ERt",ERt);
  datatree->SetBranchAddress("ERYield",ERYield);

  //make a file for output streaming
  ofstream dmcout(outfile.c_str());
  dmcout << "EV" << "\t" << "D3" << "\t" << "Type" << "\t" << "X3" << "\t" << "Y3" << "\t" << "Z3" << endl;

  int count = 0;
  //loop through all the events
  for(int i=0;i<datatree->GetEntries();i++){
    datatree->GetEvent(i);
    //cout << "NRhit: " << NRhit << " ERhit: " << ERhit << endl;
    
    double edepp=0;

    //make a buffer stream
    ostringstream evbuff;


    //get the total energy deposited in NR and ER at early or late times
    //veto the event if got an ERhit or NRhit when opposite class requested 
    int nrnum=0;
    int ernum=0;
    for(int j=0;j<NRhit;j++){
      //cout << "Looping for: " << NRhit << " nrhits" << " at time " << NRt[j] << " nanoseconds " << "> " << sec*1e9 << " and " << !delayed << " total " << (NRt>(sec*1.0e9))<< endl;
      if((NRt[j]<(sec*1e9)) && !delayed) continue;
      if((NRt[j]>(sec*1e9)) && delayed ) continue;

      nrnum++;

      edepp+=NRedep[j];

      //fill buffer stream
      evbuff << count << "\t" << NRedep[j] << "\t" << 10000 << "\t" << NRx[j] << "\t" << NRy[j] << "\t" << NRz[j] << endl;
    }

    for(int j=0;j<ERhit;j++){
      if((ERt[j]<(sec*1e9)) && !delayed) continue;
      if((ERt[j]>(sec*1e9)) && delayed) continue;

      ernum++;

      edepp+=ERedep[j];
      
      //fill buffer stream
      evbuff << count << "\t" << ERedep[j] << "\t" << 11 << "\t" << ERx[j] << "\t" << ERy[j] << "\t" << ERz[j] << endl;
    }

    //fill and print
    bool NRinRange = (nrnum>=NRhitsMin && nrnum<NRhitsMax);
    bool ERinRange = (ernum>=ERhitsMin && ernum<ERhitsMax);
    bool TotalEinRange = (edepp>=TotalEMin && edepp<TotalEMax);
    //cout << "NRinRange: " << NRinRange << "\tERinRange: " << ERinRange << "\tTotalEinRange: " << TotalEinRange << endl;
    if(NRinRange && ERinRange && TotalEinRange && count<maxev){
      h->Fill(edepp/1000);
      dmcout << evbuff.str();
      count++;
    }

  }

  dmcout.close();
  return h;
}

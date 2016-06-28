#include "physicalFunctions.C"

TH1D *getDetNREDist(int zip,int bins=100,double xmin=0.0, double xmax=5.0,bool singles=false, string file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/sbbe_trees.root",bool resave=false)
{
  //open the file and get the tree
  cout << "Opening file: " << file << endl;
  TFile *f = new TFile(file.c_str());
  if(!f){
    cout << "getDetNREDist: ERROR! could not read file" << endl;
    return NULL;
  }
  TTree *t = f->Get(Form("sumzip%d",zip));
  cout << "Tree is: " << t << endl;
  t->Print();

  //create a histogram
  TH1D *h = new TH1D(Form("edep_zip%d",zip),Form("edep_zip%d",zip),bins,xmin,xmax);
  cout << "Histogram is: " << h << endl;

  //fill the histogram
  if(singles){
    t->Draw(Form("edepNR/1000>>edep_zip%d",zip),"NRhit==1 && ERhit==0","goff");
  }
  else{
    t->Draw(Form("edepNR/1000>>edep_zip%d",zip),"NRhit>1 && ERhit==0","goff");
  }

  return h;
}
TH1D *getDetEREDist(int zip,int bins=100,double xmin=0.0, double xmax=5.0,bool pure=true, string file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/sbbe_trees.root",bool resave=false)
{
  //open the file and get the tree
  cout << "Opening file: " << file << endl;
  TFile *f = new TFile(file.c_str());
  if(!f){
    cout << "getDetEREDist: ERROR! could not read file" << endl;
    return NULL;
  }
  TTree *t = f->Get(Form("sumzip%d",zip));
  cout << "Tree is: " << t << endl;
  t->Print();

  //create a histogram
  TH1D *h = new TH1D(Form("edep_zip%d",zip),Form("edep_zip%d",zip),bins,xmin,xmax);
  cout << "Histogram is: " << h << endl;

  //fill the histogram
  if(pure){
    t->Draw(Form("edepER/1000>>edep_zip%d",zip),"NRhit==0 && ERhit>0","goff");
  }
  else{
    t->Draw(Form("edepER/1000>>edep_zip%d",zip),"NRhit>0 && ERhit>0","goff");
  }

  return h;
}
TH2D *getDetYieldEDist(int zip,double volts=4,double eres=100, double pres=100,int bins=100,double xmin=0.0, double xmax=5.0, string file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/sbbe_trees.root",bool resave=false)
{
  //some parameters that should eventually be inputs
  //double volts=4;
  //double eres=100; //in eV
  //double pres=100; //in eV
  int earlylate=0; //0=both, 1=early, 2=late

  //open the file and get the tree
  cout << "Opening file: " << file << endl;
  TFile *f = new TFile(file.c_str());
  if(!f){
    cout << "getDetYieldEDist: ERROR! could not read file" << endl;
    return NULL;
  }
  TTree *datatree = f->Get(Form("sumzip%d",zip));
  cout << "Tree is: " << datatree << endl;
  datatree->Print();

  //create a histogram
  TH2D *h = new TH2D(Form("yieldedep_zip%d",zip),Form("yieldedep_zip%d",zip),bins,xmin,xmax,bins,0.0,1.5);
  //TH1D *h = new TH1D(Form("yieldedep_zip%d",zip),Form("yieldedep_zip%d",zip),bins,xmin,xmax);
  cout << "Histogram is: " << h << endl;

  //random number generator for resolutions
  //https://root.cern.ch/doc/master/classTRandom.html
  TRandom3 rand;

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

  //loop through all the events
  for(int i=0;i<datatree->GetEntries();i++){
    datatree->GetEvent(i);
    //cout << "NRhit: " << NRhit << " ERhit: " << ERhit << endl;
    
    //if(NRhit>1) continue;

    //need to store charge and phonon energy deposition, recoil energy and yield
    //basic programme:
    //0. get charge energy by lindhard (for different species)
    //1. get total phonon energy with voltage
    //2. add resolutions
    //3. construct recoil energy and yield
    double edepq=0,edepp=0,erecoil=0,yield=0;

    Double_t lindhard_params[6]; //germanium
    lindhard_params[0]=3.0;
    lindhard_params[1]=0.15;
    lindhard_params[2]=0.7;
    lindhard_params[3]=0.6;
    lindhard_params[4]=32;
    lindhard_params[5]=73;

    //get the total energy deposited in NR and ER at early or late times
    for(int j=0;j<NRhit;j++){
      //cout << "Computing NRedep " << NRedep[j] << " lindhard contribution " << lindhard((NRedep+j),lindhard_params) << " for index " << j << endl;
      edepp+=NRedep[j];
      edepq+=NRedep[j]*lindhard((NRedep+j),lindhard_params); //pass pointer for first arg
    }
    for(int j=0;j<ERhit;j++){
      edepp+=ERedep[j];
      edepq+=ERedep[j];
    }

    //add Luke
    edepp += volts*(edepq/3);

    //add resolution
    edepp = rand.Gaus(edepp,pres);
    edepq = rand.Gaus(edepq,eres);

    //convert to yield
    erecoil = edepp - volts*(edepq/3);
    yield = edepq/erecoil;
    h->Fill(erecoil,yield);
    //h->Fill(erecoil);

  }

  return h;
}
TH1D *getDetPtDist(int zip,double volts=70,double pres=10,int bins=100,double xmin=0.0, double xmax=5.0,bool singles=false, bool indivyield=true, string file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/sbbe_trees.root",bool resave=false)
{
  //some parameters that should eventually be inputs
  //double volts=70;
  //double eres=10; //in eV
  //double pres=10; //in eV
  int earlylate=0; //0=both, 1=early, 2=late

  //open the file and get the tree
  cout << "Opening file: " << file << endl;
  TFile *f = new TFile(file.c_str());
  if(!f){
    cout << "getDetYieldEDist: ERROR! could not read file" << endl;
    return NULL;
  }
  TTree *datatree = f->Get(Form("sumzip%d",zip));
  cout << "Tree is: " << datatree << endl;
  datatree->Print();

  //create a histogram
  TH1D *h = new TH1D(Form("yieldedep_zip%d",zip),Form("yieldedep_zip%d",zip),bins,xmin,xmax);
  cout << "Histogram is: " << h << endl;

  //random number generator for resolutions
  //https://root.cern.ch/doc/master/classTRandom.html
  TRandom3 rand;

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

  //loop through all the events
  for(int i=0;i<datatree->GetEntries();i++){
    datatree->GetEvent(i);
    //cout << "NRhit: " << NRhit << " ERhit: " << ERhit << endl;
    
    if(ERhit>0) continue;

    if(singles && NRhit>1) continue;

    if(!singles && NRhit==1) continue;

    //need to store charge and phonon energy deposition, recoil energy and yield
    //basic programme:
    //0. get charge energy by lindhard (for different species)
    //1. get total phonon energy with voltage
    //2. add resolutions
    //3. construct recoil energy and yield
    double edepq=0,edepp=0,erecoil=0,yield=0;

    Double_t lindhard_params[6]; //germanium
    lindhard_params[0]=3.0;
    lindhard_params[1]=0.15;
    lindhard_params[2]=0.7;
    lindhard_params[3]=0.6;
    lindhard_params[4]=32;
    lindhard_params[5]=73;

    //get the total energy deposited in NR and ER at early or late times
    for(int j=0;j<NRhit;j++){
      //cout << "Computing NRedep " << NRedep[j] << " lindhard contribution " << lindhard((NRedep+j),lindhard_params) << " for index " << j << endl;
      edepp+=NRedep[j];
      if(indivyield)
        edepq+=NRedep[j]*lindhard((NRedep+j),lindhard_params); //pass pointer for first arg
    }
    Double_t linde[1];
    linde[0]=edepp;
    if(!indivyield)
      edepq+=edepp*lindhard(linde,lindhard_params);


    //add Luke
    edepp += volts*(edepq/3);

    //add resolution
    edepp = rand.Gaus(edepp,pres);

    //convert to yield
    h->Fill(edepp);

  }

  return h;
}
TH1D *getDetNRMultEDist(int zip,int bins=100,double xmin=0.0, double xmax=5.0,bool singles=false, string file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/sbbe_trees.root",bool resave=false)
{
  //open the file and get the tree(s)
  cout << "Opening file: " << file << endl;
  TFile *f = new TFile(file.c_str());
  if(!f){
    cout << "getDetNREDist: ERROR! could not read file" << endl;
    return NULL;
  }
  TTree *t = f->Get(Form("sumzip%d",zip));
  cout << "Tree is: " << t << endl;
  t->Print();

  //get another tree
  int count = t->GetEntries(); //make sure same entries
  int treecount = 0;
  string cut = "";
  for(int i=1;i<16;i++){
    if(i!=zip){
      TTree *tnew = f->Get(Form("sumzip%d",i));
      if(tnew->GetEntries()!= count){
         cout << "getDetNRMultEDist: ERROR! one zip doesn't have the right number of events" << endl;
         return NULL;
       }
      //friend the tree
      t->AddFriend(tnew,Form("t%d",i));
      if(treecount==0){
        cut+=Form("t%d.ERhit>0 ",i);
      }
      else{
        cut+=Form(" || t%d.ERhit>0 ",i);
      }
      treecount++;
    }
  }


  //create a histogram
  TH1D *h = new TH1D(Form("edep_zip%d",zip),Form("edep_zip%d",zip),bins,xmin,xmax);

  //cut on the friended tree
  //string cut;
  //cut+=Form("t1.ERhit>0");

  //fill the histogram
  if(singles){
    t->Draw(Form("edepNR/1000>>edep_zip%d",zip),Form("NRhit==1 && ERhit==0 && (%s)",cut.c_str()),"goff");
  }
  else{
    t->Draw(Form("edepNR/1000>>edep_zip%d",zip),Form("NRhit>1 && ERhit==0 && (%s)",cut.c_str()),"goff");
  }

  return h;
}

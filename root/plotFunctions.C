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

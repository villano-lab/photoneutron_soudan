TH1D *getHist(int zip,int bins=100,double xmin=0.0,double xmax=5.0,bool singles=false, string file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/sbbe_trees.root")
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

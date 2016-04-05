TTree *projectSumDepEv(TChain *ch,string cut,int maxev=1000000)
{
  Int_t nev;

  //total number of events
  Long64_t ntot = ch->GetEntries();
  cout << "Processing " << TMath::Min((Long64_t)maxev,(Long64_t)ntot) << " events" << endl;
  ch->SetEstimate(ntot);

  //write out the NR and ER cuts as strings
  string ERcut = "(allzips.PType==11 || allzips.PType==-11) || allzips.PType==22";
  string NRcut = "allzips.PType==2112 || allzips.PType-allzips.PType%10000>1";

  //make new TTree with total edepNR, edepER, nhit, NRhit, ERhit, positions?
  Long64_t nhit,NRhit,ERhit;
  Double_t edepNR,edepER;
   
  // create tree and the ROOT file to store it
  TTree *datatree = new TTree("edeptree","energy deposition data");
   
  // create branches in tree for each data column
  datatree->Branch("nhit",&nhit,"nhit/L");
  datatree->Branch("NRhit",&NRhit,"NRhit/L");
  datatree->Branch("ERhit",&ERhit,"ERhit/L");
  datatree->Branch("edepNR",&edepNR,"edepNR/D");
  datatree->Branch("edepER",&edepER,"edepER/D");

  //get meaningful quantities from the tree/chain
  string NRhitstr = "Sum$(1*(("+NRcut+") && ("+cut+")))";
  string ERhitstr = "Sum$(1*(("+ERcut+") && ("+cut+")))";
  string edepNRstr = "Sum$(allzips.Edep*(("+NRcut+") && ("+cut+")))";
  string edepERstr = "Sum$(allzips.Edep*(("+ERcut+") && ("+cut+")))";

  //do 100M at a time
  int iterations=ntot/maxev;

  if(ntot%maxev!=0) iterations++;

  for(int j=0;j<iterations;j++){
    cout << "iteration " << j+1 << " of " << iterations << endl;
    ch->Draw(Form("%s:%s:%s:%s",NRhitstr.c_str(),ERhitstr.c_str(),edepNRstr.c_str(),edepERstr.c_str()),Form("Entry$<%d && Entry$>=%d",maxev*(j+1),maxev*j),"goff");
    Long64_t nevents = ch->GetSelectedRows();
    Double_t *NRhitvec = ch->GetV1();
    Double_t *ERhitvec = ch->GetV2();
    Double_t *edepNRvec = ch->GetV3();
    Double_t *edepERvec = ch->GetV4();

    //loop through the lists and fill the tree;
    for(Long_t i=0; i<TMath::Min((Long64_t)maxev,nevents); i++){
      nhit=0;NRhit=0;ERhit=0;
      edepNR=0;edepER=0;

      NRhit=NRhitvec[i];
      ERhit=ERhitvec[i];
      edepNR=edepNRvec[i];
      edepER=edepERvec[i];
      nhit=NRhit+ERhit;
      datatree->Fill();
    }

    free(NRhitvec);
    free(ERhitvec);
    free(edepNRvec);
    free(edepERvec);

  }


  return datatree;
}

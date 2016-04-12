
Double_t lindhard(Double_t *x, Double_t *par){

  //plausible values for parameters
  //root [5] f->SetParameter(0,3.0);
  //root [6] f->SetParameter(1,0.15);
  //root [7] f->SetParameter(2,0.7);
  //root [8] f->SetParameter(3,0.6);
  //root [9] f->SetParameter(4,32); //Z (germanium)
  //root [10] f->SetParameter(5,73); //A (average germanium)


  Double_t Z = par[4];
  Double_t A = par[5];
  Double_t xvar = x[0]/1000.0; //change units from eV to keV

  Double_t kscale = 0.133*pow(Z,(2/3.0))*pow(A,(-1/2.0));
  Double_t escale = 11.5*pow(Z,(-7/3.0));
  Double_t g = par[0]*pow((escale*xvar),par[1]) + par[2]*pow((escale*xvar),par[3]) + escale*xvar;
  return kscale*g/(1+kscale*g);

}

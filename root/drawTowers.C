
TH2Poly *towerLayout()
{

  Double_t x0[] = {4.5593,2.27965,-2.27965,-4.5593,-2.27965,2.27965,4.5593}; 
  Double_t y0[] = {0,3.9485,3.9485,0,-3.9485,-3.9485,0}; 
  TH2Poly *zips = new TH2Poly();

  //store the x and y shifts for the towers in TOWER NUMBER ORDER
  vector<double> shiftx,shifty;
  shiftx.push_back(8.1571);
  shifty.push_back(-4.7095);
  shiftx.push_back(8.1571);
  shifty.push_back(4.7095);
  shiftx.push_back(0);
  shifty.push_back(-9.4190);
  shiftx.push_back(0);
  shifty.push_back(0);
  shiftx.push_back(0);
  shifty.push_back(9.4190);

  //construct the five zips
  for(int i=0;i<5;i++){
    Double_t x[7],y[7];
    for(int j=0;j<7;j++){x[j]=x0[j]+shiftx[i]; y[j]=y0[j]+shifty[i];}
    zips->AddBin(6,x,y);
  }

  return zips;
}


TH2Poly *towerLayout()
{

  Double_t x1p[] = {4.5593,2.27965,-2.27965,-4.5593,-2.27965,2.27965,4.5593}; 
  Double_t y1p[] = {0,3.9485,3.9485,0,-3.9485,-3.9485,0}; 
  TH2Poly *zips = new TH2Poly();
  zips->AddBin(6,x1p,y1p);

  return zips;
}

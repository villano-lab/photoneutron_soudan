#include <iostream>
#include <string>

void demoVoltageMap()
{
  //open file and retrieve histos
  TFile* f = new TFile("voltagemap_1extrap.root");

  TH3D* voltageMapWeighted = f->Get("voltageMapWeighted");
  voltageMapWeighted->SetStats(kFALSE);

  TH3D* voltageMapCount = f->Get("voltageMapCount");
  voltageMapCount->SetStats(kFALSE);

  //make voltage map by dividing weighted histo bins by number of counts 
  //(to compute average voltage in each bin)
  //note that based on the way the histo was setup, the number of counts is always 1 
  //so this step is just to be careful there wasn't an unaccounted for boundary issue 
  //when generating the original weighted map.
  TH3D* voltageMap3D = (TH3D*)voltageMapWeighted->Clone();
  int nbinsX3D = voltageMapWeighted->GetNbinsX();
  int nbinsY3D = voltageMapWeighted->GetNbinsY();
  int nbinsZ3D = voltageMapWeighted->GetNbinsZ();
  int nbins3D = (nbinsX3D+2)*(nbinsY3D+2)*(nbinsZ3D+2); //plus 2 is for overflow and underflow bins

  for(int binItr3D = 1; binItr3D <= nbins3D; binItr3D++)
  {
    double weightedBinContent3D = voltageMap3D->GetBinContent(binItr3D);
    double nSamples3D = voltageMapCount->GetBinContent(binItr3D);
    double mapValue3D = (nSamples3D > 0 ? (weightedBinContent3D/nSamples3D) : 0);

    if(nSamples3D > 1)
      cout <<"nSamples3D = " << nSamples3D << endl;

    //now overwrite the bin content 
    voltageMap3D->SetBinContent(binItr3D, mapValue3D);
  }

  //ROOT says interpolation function only works for values within 
  //first and last binCtr
  TAxis* xaxis = voltageMap3D->GetXaxis();
  double xmin = xaxis->GetBinCenter(xaxis->GetFirst());
  double xmax = xaxis->GetBinCenter(xaxis->GetLast());
 
  TAxis* yaxis = voltageMap3D->GetYaxis();
  double ymin = yaxis->GetBinCenter(yaxis->GetFirst());
  double ymax = yaxis->GetBinCenter(yaxis->GetLast());

  TAxis* zaxis = voltageMap3D->GetZaxis();
  double zmin = zaxis->GetBinCenter(zaxis->GetFirst());
  double zmax = zaxis->GetBinCenter(zaxis->GetLast());

  cout <<"xmin = " << xmin <<", xmax = " << xmax 
       <<"\nymin = " << ymin <<", ymax = " << ymax 
       <<"\nzmin = " << zmin <<", zmax = " << zmax 
       << endl;


  //now fill some histograms to test interpolation
  int ntestbinx = 2000;
  int ntestbiny = 2000;

  TH2D* test_interpLow = new TH2D("voltageMapInterpLow", "voltageMapInterpLow, z=0.5", ntestbinx, xmin, xmax, ntestbiny, ymin, ymax);
  test_interpLow->SetYTitle("Y [mm]");
  test_interpLow->SetXTitle("X [mm]");
  test_interpLow->SetStats(kFALSE);

  TH2D* test_interpMid = new TH2D("voltageMapInterpMid", "voltageMapInterpMid, z=10.0", ntestbinx, xmin, xmax, ntestbiny, ymin, ymax);
  test_interpMid->SetYTitle("Y [mm]");
  test_interpMid->SetXTitle("X [mm]");
  test_interpMid->SetStats(kFALSE);

  TH2D* test_interpHigh = new TH2D("voltageMapInterpHigh", "voltageMapInterpHigh, z=24.5", ntestbinx, xmin, xmax, ntestbiny, ymin, ymax);
  test_interpHigh->SetYTitle("Y [mm]");
  test_interpHigh->SetXTitle("X [mm]");
  test_interpHigh->SetStats(kFALSE);

  for(int binItr = 1; binItr <= (2+ntestbinx)*(2+ntestbiny); binItr++)
  {
    int binx, biny, binz;
    test_interpLow->GetBinXYZ(binItr, binx, biny, binz);
    
    if(binx*biny == 0 || binx > ntestbinx || biny > ntestbiny)
      continue;

    //cout <<"binx, biny, binz = " << binx <<", " << biny <<", " << binz << endl;

    double xval = (test_interpLow->GetXaxis())->GetBinCenter(binx);
    double yval = (test_interpLow->GetYaxis())->GetBinCenter(biny);
    
    //extract values from voltage map
    double mapval5 = voltageMap3D->Interpolate(xval, yval, 0.5); //interpolate near z=0.0 plane
    double mapval10 = voltageMap3D->Interpolate(xval, yval, 10.0); //interpolate near z=10 plane
    double mapval25 = voltageMap3D->Interpolate(xval, yval, 24.5); //interpolate near z=25 plane
    //cout <<"xval, yval = " << xval <<", " << yval << endl;

    //store in histogram so that we can plot it
    test_interpLow->SetBinContent(binItr, mapval5);
    test_interpMid->SetBinContent(binItr, mapval10);
    test_interpHigh->SetBinContent(binItr, mapval25);

  }

  //A cross check 
  TH2D* projectCount_yx = (TH2D*)voltageMapWeighted->Project3D("yx");
  projectCount_yx->SetStats(kFALSE);

  //Make plots to check mapping
  TCanvas* can1 = new TCanvas("can1", "can1", 800, 800);
  can1->Divide(2,2);

  can1->cd(1);
  projectCount_yx->SetMaximum(150);
  projectCount_yx->SetMinimum(0);
  projectCount_yx->Draw("colz");

  can1->cd(2);
  test_interpLow->SetMinimum(0.0);
  test_interpLow->SetMaximum(1.2);
  test_interpLow->Draw("colz");

  can1->cd(3);
  test_interpMid->SetMinimum(0.0);
  test_interpMid->SetMaximum(1.2);
  test_interpMid->Draw("colz");

  can1->cd(4);
  test_interpHigh->SetMinimum(0.0);
  test_interpHigh->SetMaximum(1.2);
  test_interpHigh->Draw("colz");

}

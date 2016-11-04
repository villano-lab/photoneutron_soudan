
#include "RootUtil.C"
#include "plotFunctions.C"

void plotNREDist(bool print=0,bool isprelim=true,int zip=1,string datatype="ybe", string ext="eps",bool resave=false)
{
  
  //let's do some calculations to make the axis size ratio correct for some specified
  //ratio of x-size to y-size.  
  int xw=810,yw=-1;
  double r=1.0;
  double xr = 0.05;
  double xl = 0.15;
  double yt = 0.05;
  double yb = 0.12;
  double padsp=0.1;

  findCanvasSize(xw,yw,1,1,r,xr,xl,yt,yb,padsp);

  gROOT->SetStyle("Plain");
  //make a canvas
  TCanvas* c1 = (TCanvas*) gROOT->GetListOfCanvases()->FindObject("c1");
  if(c1){
    c1->Close();
    c1 = new TCanvas("c1","Nuclear Recoil Energy Scale",200,10,xw,yw);
  }
  else
    c1 = new TCanvas("c1","Nuclear Recoil Energy Scale",200,10,xw,yw);


  c1->cd();
  //log-log plot
  c1->SetLogy();
  //c1->SetLogx();
  c1->SetTickx();
  c1->SetTicky();
  c1->SetLeftMargin(xl);
  c1->SetBottomMargin(yb);
  c1->SetTopMargin(yt);
  c1->SetRightMargin(xr);
  c1->SetGrid(1,1);
  TLegend *leg;
  leg = new TLegend(0.50,0.15,0.90,0.30);

  double px=1E-11,py=1E-8;
  //size of x-axis
  double xmin=0.0;
  double xmax=60.0;
  double ymin=1.0e-1;
  double ymax=1e4;
  string file;
  if(datatype=="ybe"){
    xmin=0.0;
    xmax=20.0;
    px=5.0;
    py=1e3;
    file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/ybe_trees.root";
  }
  else if(datatype=="sbbe"){
    xmin=0.0;
    xmax=5.0;
    px=1.0;
    py=1e3;
    file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/sbbe_trees.root";
  }
  else{
    cout << "Unknown data type for plotting" << endl;
    return;
  }

  gStyle->SetLineWidth(2);
  gStyle->SetHistLineWidth(1);
  gStyle->SetGridWidth(0);

  //make a frame for this histogram
  TH1* frame1;
  frame1 = makeFrame(xmin,xmax,ymin,ymax,10,"Total NR Deposited Energy [keV]","counts",1.2,0.8);
  if(isprelim){
    TText *prelim = new TText(px,py,"preliminary");
    prelim->SetTextAngle(0);
    prelim->SetTextSize(0.1);
    prelim->SetTextColor(17);
    prelim->SetTextFont(42);
    frame1->GetListOfFunctions()->Add(prelim);
  }
  frame1->Draw();
  //TAxis *xaxis = frame1->GetXaxis();
  //xaxis->SetNoExponent(true);

  //make some new gstyles for thick lines
  //https://root.cern.ch/root/html/TStyle.html#TStyle:SetLineStyleString
  gStyle->SetLineStyleString(12,"36 36"); //dashed
  gStyle->SetLineStyleString(13,"12 24"); //dotted
  gStyle->SetLineStyleString(14,"36 48 12 48"); //dash-dot
  gStyle->SetLineStyleString(15,"60 36 12 36 12 36 12 36"); //dash-triple-dot


  TH1D *h = getDetNREDist(zip,100,xmin,xmax,false,file);
  TH1D *hs = getDetNREDist(zip,100,xmin,xmax,true,file);
  h->SetLineColor(kBlue);
  hs->SetLineColor(kRed);
  TH1D *hsum = new TH1D();
  *hsum = *h;
  hsum->Add(hs);
  hsum->SetLineColor(kBlack);
  hsum->SetLineWidth(2);
  leg->AddEntry(h,Form("iZIP %d: NR multiples",zip),"l");
  leg->AddEntry(hs,Form("iZIP %d: NR singles",zip),"l");
  leg->AddEntry(hsum,Form("iZIP %d: all NR deposits",zip),"l");

  h->Draw("same");
  hs->Draw("same");
  hsum->Draw("same");
  //print up legend
  //leg->SetHeader(Form("%d GeV %s primaries ",energy,getFullPartName(part).c_str()));
  leg->SetFillColor(0);
  leg->SetLineWidth(2);
  leg->SetTextFont(42);
  leg->SetTextSize(.03);
  leg->Draw("same");

  //output the file
  if(print){
    plotCanvas(c1,Form("figures/NREDist_%s_zip%d",datatype.c_str(),zip),ext);
  }
}
void plotAllEDist(bool print=0,bool isprelim=true,int zip=1,string datatype="ybe",string file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/ybe_trees.root",string postfix="", string ext="eps",bool resave=false)
{
  
  //let's do some calculations to make the axis size ratio correct for some specified
  //ratio of x-size to y-size.  
  int xw=810,yw=-1;
  double r=1.0;
  double xr = 0.05;
  double xl = 0.15;
  double yt = 0.05;
  double yb = 0.12;
  double padsp=0.1;

  findCanvasSize(xw,yw,1,1,r,xr,xl,yt,yb,padsp);

  gROOT->SetStyle("Plain");
  //make a canvas
  TCanvas* c1 = (TCanvas*) gROOT->GetListOfCanvases()->FindObject("c1");
  if(c1){
    c1->Close();
    c1 = new TCanvas("c1","Nuclear Recoil Energy Scale",200,10,xw,yw);
  }
  else
    c1 = new TCanvas("c1","Nuclear Recoil Energy Scale",200,10,xw,yw);


  c1->cd();
  //log-log plot
  c1->SetLogy();
  //c1->SetLogx();
  c1->SetTickx();
  c1->SetTicky();
  c1->SetLeftMargin(xl);
  c1->SetBottomMargin(yb);
  c1->SetTopMargin(yt);
  c1->SetRightMargin(xr);
  c1->SetGrid(1,1);
  TLegend *leg;
  leg = new TLegend(0.50,0.15,0.90,0.30);

  double px=1E-11,py=1E-8;
  //size of x-axis
  double xmin=0.0;
  double xmax=60.0;
  double ymin=1.0e-1;
  double ymax=1e4;
  if(datatype=="ybe"){
    xmin=0.0;
    xmax=20.0;
    px=5.0;
    py=1e3;
    //file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/ybe_trees_160405_all.root";
  }
  else if(datatype=="sbbe"){
    xmin=0.0;
    xmax=5.0;
    px=1.0;
    py=1e3;
    //file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/sbbe_trees_160405_all.root";
  }
  else if(datatype=="lewide"){
    xmin=0.0;
    xmax=2.0;
    px=0.876553;
    py=2069.57;
    //file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/sbbe_trees_160405_all_wtime_v2.root";
  }
  else if(datatype=="ledwide"){
    xmin=0.0;
    xmax=4.0;
    px=0.8;
    py=1e3;
    //file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/sbbe_trees_160405_all_wtime_v2.root";
  }
  
  else{
    cout << "Unknown data type for plotting" << endl;
    return;
  }

  gStyle->SetLineWidth(2);
  gStyle->SetHistLineWidth(1);
  gStyle->SetGridWidth(0);

  //make a frame for this histogram
  TH1* frame1;
  frame1 = makeFrame(xmin,xmax,ymin,ymax,10,"Deposited Energy [keV]","counts",1.2,0.8);
  if(isprelim){
    TText *prelim = new TText(px,py,"preliminary");
    prelim->SetTextAngle(0);
    prelim->SetTextSize(0.1);
    prelim->SetTextColor(17);
    prelim->SetTextFont(42);
    frame1->GetListOfFunctions()->Add(prelim);
  }
  frame1->Draw();
  //TAxis *xaxis = frame1->GetXaxis();
  //xaxis->SetNoExponent(true);

  //make some new gstyles for thick lines
  //https://root.cern.ch/root/html/TStyle.html#TStyle:SetLineStyleString
  gStyle->SetLineStyleString(12,"36 36"); //dashed
  gStyle->SetLineStyleString(13,"12 24"); //dotted
  gStyle->SetLineStyleString(14,"36 48 12 48"); //dash-dot
  gStyle->SetLineStyleString(15,"60 36 12 36 12 36 12 36"); //dash-triple-dot


  TH1D *hnr = getDetNREDist(zip,100,xmin,xmax,false,file);
  hnr->SetLineWidth(3);
  TH1D *hnrs = getDetNREDist(zip,100,xmin,xmax,true,file);
  hnrs->SetLineWidth(3);
  TH1D *her = getDetEREDist(zip,100,xmin,xmax,true,file);
  her->SetLineWidth(3);
  TH1D *hmix = getDetEREDist(zip,100,xmin,xmax,false,file);
  hmix->SetLineWidth(3);
  hnr->SetLineColor(kBlue);
  hnrs->SetLineColor(kRed);
  her->SetLineColor(kGreen);
  hmix->SetLineColor(kMagenta);
  TH1D *hsum = new TH1D();
  *hsum = *hnr;
  hsum->Add(hnrs);
  hsum->Add(her);
  hsum->Add(hmix);
  hsum->SetLineColor(kBlack);
  hsum->SetLineWidth(3);
  leg->AddEntry(hnr,Form("iZIP %d: NR multiples",zip),"l");
  leg->AddEntry(hnrs,Form("iZIP %d: NR singles",zip),"l");
  leg->AddEntry(her,Form("iZIP %d: ER all",zip),"l");
  leg->AddEntry(hmix,Form("iZIP %d: ER/NR mix",zip),"l");
  leg->AddEntry(hsum,Form("iZIP %d: all deposits",zip),"l");

  hnr->Draw("same");
  hnrs->Draw("same");
  her->Draw("same");
  hmix->Draw("same");
  hsum->Draw("same");
  //print up legend
  //leg->SetHeader(Form("%d GeV %s primaries ",energy,getFullPartName(part).c_str()));
  leg->SetFillColor(0);
  leg->SetLineWidth(2);
  leg->SetTextFont(42);
  leg->SetTextSize(.03);
  leg->Draw("same");

  //output the file
  if(print){
    plotCanvas(c1,Form("figures/AllEDist%s_%s_zip%d",postfix.c_str(),datatype.c_str(),zip),ext);
  }
}
void plotPtDist(bool print=0,bool isprelim=true,int zip=1,string datatype="ybe",string file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/ybe_trees.root",string postfix="", string ext="eps",bool resave=false)
{
  
  //let's do some calculations to make the axis size ratio correct for some specified
  //ratio of x-size to y-size.  
  int xw=810,yw=-1;
  double r=1.0;
  double xr = 0.05;
  double xl = 0.15;
  double yt = 0.05;
  double yb = 0.12;
  double padsp=0.1;

  findCanvasSize(xw,yw,1,1,r,xr,xl,yt,yb,padsp);

  gROOT->SetStyle("Plain");
  //make a canvas
  TCanvas* c1 = (TCanvas*) gROOT->GetListOfCanvases()->FindObject("c1");
  if(c1){
    c1->Close();
    c1 = new TCanvas("c1","Nuclear Recoil Energy Scale (Pt)",200,10,xw,yw);
  }
  else
    c1 = new TCanvas("c1","Nuclear Recoil Energy Scale (Pt)",200,10,xw,yw);


  c1->cd();
  //log-log plot
  c1->SetLogy();
  //c1->SetLogx();
  c1->SetTickx();
  c1->SetTicky();
  c1->SetLeftMargin(xl);
  c1->SetBottomMargin(yb);
  c1->SetTopMargin(yt);
  c1->SetRightMargin(xr);
  c1->SetGrid(1,1);
  TLegend *leg;
  leg = new TLegend(0.25,0.15,0.90,0.30);

  double px=1E-11,py=1E-8;
  //size of x-axis
  double xmin=0.0;
  double xmax=60.0;
  double ymin=1.0e-1;
  double ymax=1e4;
  if(datatype=="ybe"){
    xmin=0.0;
    xmax=100.0;
    px=25.0*1000;
    py=1e3;
    //file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/ybe_trees_160405_all_wtime_v2.root";
  }
  else if(datatype=="sbbe"){
    xmin=0.0;
    xmax=10.0;
    px=2.0*1000;
    py=1e3;
    //file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/sbbe_trees_160405_all_wtime_v2.root";
  }
  else if(datatype=="lewide"){
    xmin=0.0;
    xmax=20.0;
    px=2.0*2000;
    py=1e3;
    //file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/sbbe_trees_160405_all_wtime_v2.root";
  }
  else if(datatype=="ledwide"){
    xmin=0.0;
    xmax=40.0;
    px=2.0*4000;
    py=1e3;
    //file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/sbbe_trees_160405_all_wtime_v2.root";
  }
  else{
    cout << "Unknown data type for plotting" << endl;
    return;
  }

  gStyle->SetLineWidth(2);
  gStyle->SetHistLineWidth(1);
  gStyle->SetGridWidth(0);

  //make a frame for this histogram
  TH1* frame1;
  frame1 = makeFrame(xmin*1000,xmax*1000,ymin,ymax,10,"Total Phonon Energy [pt] (eV)","counts",1.2,0.8);
  if(isprelim){
    TText *prelim = new TText(px,py,"preliminary");
    prelim->SetTextAngle(0);
    prelim->SetTextSize(0.1);
    prelim->SetTextColor(17);
    prelim->SetTextFont(42);
    frame1->GetListOfFunctions()->Add(prelim);
  }
  frame1->Draw();
  //TAxis *xaxis = frame1->GetXaxis();
  //xaxis->SetNoExponent(true);

  //make some new gstyles for thick lines
  //https://root.cern.ch/root/html/TStyle.html#TStyle:SetLineStyleString
  gStyle->SetLineStyleString(12,"36 36"); //dashed
  gStyle->SetLineStyleString(13,"12 24"); //dotted
  gStyle->SetLineStyleString(14,"36 48 12 48"); //dash-dot
  gStyle->SetLineStyleString(15,"60 36 12 36 12 36 12 36"); //dash-triple-dot


  TH1D *hnrs = getDetPtDist(zip,70,10,300,0.0,xmax*1000,true,true,file,false);
  TH1D *hnr = getDetPtDist(zip,70,10,300,0.0,xmax*1000,false,true,file,false);
  TH1D *hnr_sing = getDetPtDist(zip,70,10,300,0.0,xmax*1000,false,false,file,false);
  hnr->SetLineColor(kBlue);
  hnrs->SetLineColor(kRed);
  hnr_sing->SetLineColor(kGreen);
  TH1D *hsum = new TH1D();
  TH1D *hsum_sing = new TH1D();
  *hsum = *hnr;
  *hsum_sing = *hnr_sing;
  hsum->Add(hnrs);
  hsum->SetLineColor(kBlack);
  hsum->SetLineWidth(2);
  hsum_sing->Add(hnrs);
  hsum_sing->SetLineColor(kBlack);
  hsum_sing->SetLineStyle(2);
  hsum_sing->SetLineWidth(2);
  leg->AddEntry(hnr,Form("iZIP %d: NR multiples",zip),"l");
  leg->AddEntry(hnrs,Form("iZIP %d: NR singles",zip),"l");
  leg->AddEntry(hnr_sing,Form("iZIP %d: NR multiples w/ naive Lindhard",zip),"l");
  leg->AddEntry(hsum,Form("iZIP %d: all NR deposits",zip),"l");
  leg->AddEntry(hsum_sing,Form("iZIP %d: all NR deposits w/ naive Lindhard",zip),"l");

  hnr->Draw("same");
  hnrs->Draw("same");
  hnr_sing->Draw("same");
  hsum->Draw("same");
  hsum_sing->Draw("same");
  //print up legend
  //leg->SetHeader(Form("%d GeV %s primaries ",energy,getFullPartName(part).c_str()));
  leg->SetFillColor(0);
  leg->SetLineWidth(2);
  leg->SetTextFont(42);
  leg->SetTextSize(.03);
  leg->Draw("same");

  //output the file
  if(print){
    plotCanvas(c1,Form("figures/PtDist%s_%s_zip%d",postfix.c_str(),datatype.c_str(),zip),ext);
  }
}
void plotYieldRecoilDist(bool print=0,bool isprelim=true,int zip=1,double volts=4, double eres=100,double pres=100,bool zoom=false,string datatype="ybe",string file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/ybe_trees.root",string postfix="", string ext="eps",bool resave=false)
{
  
  //let's do some calculations to make the axis size ratio correct for some specified
  //ratio of x-size to y-size.  
  int xw=810,yw=-1;
  double r=1.0;
  double xr = 0.05;
  double xl = 0.15;
  double yt = 0.05;
  double yb = 0.12;
  double padsp=0.1;

  findCanvasSize(xw,yw,1,1,r,xr,xl,yt,yb,padsp);

  gROOT->SetStyle("Plain");
  //make a canvas
  TCanvas* c1 = (TCanvas*) gROOT->GetListOfCanvases()->FindObject("c1");
  if(c1){
    c1->Close();
    c1 = new TCanvas("c1","Yield vs. Recoil Energy Scale ",200,10,xw,yw);
  }
  else
    c1 = new TCanvas("c1","Yield vs. Recoil Energy Scale ",200,10,xw,yw);


  c1->cd();
  //log-log plot
  //c1->SetLogy();
  //c1->SetLogx();
  c1->SetTickx();
  c1->SetTicky();
  c1->SetLeftMargin(xl);
  c1->SetBottomMargin(yb);
  c1->SetTopMargin(yt);
  c1->SetRightMargin(xr);
  c1->SetGrid(1,1);
  TLegend *leg;
  leg = new TLegend(0.25,0.15,0.90,0.30);

  double px=1E-11,py=1E-8;
  //size of x-axis
  double xmin=0.0;
  double xmax=60.0;
  double ymin=0.0;
  double ymax=1.5;
  int nbins=300;
  if(datatype=="ybe"){
    xmin=0.0;
    xmax=100.0;
    px=25.0*1000;
    py=0.6;
    if(zoom){
      xmax=30.0;
      ymax=0.4;
      nbins=600;
      px=10*1000;
      py=0.3;
    }
    //file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/ybe_trees_160405_all_wtime_v2.root";
  }
  else if(datatype=="sbbe"){
    xmin=0.0;
    xmax=100.0;
    px=25.0*1000;
    py=0.6;
    if(zoom){
      xmax=4.0;
      ymax=0.4;
      nbins=600;
      px=1*1000;
      py=0.3;
    }
    //file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/sbbe_trees_160405_all_wtime_v2.root";
  }
  else{
    cout << "Unknown data type for plotting" << endl;
    return;
  }

  gStyle->SetLineWidth(2);
  gStyle->SetHistLineWidth(1);
  gStyle->SetGridWidth(0);

  //make a frame for this histogram
  TH1* frame1;
  frame1 = makeFrame(xmin*1000,xmax*1000,ymin,ymax,10,"Recoil Energy [pr] (keV)","Ionization Yield",1.2,0.8);
  if(isprelim){
    TText *prelim = new TText(px,py,"preliminary");
    prelim->SetTextAngle(0);
    prelim->SetTextSize(0.1);
    prelim->SetTextColor(17);
    prelim->SetTextFont(42);
    frame1->GetListOfFunctions()->Add(prelim);
  }
  frame1->Draw();
  //TAxis *xaxis = frame1->GetXaxis();
  //xaxis->SetNoExponent(true);

  //make some new gstyles for thick lines
  //https://root.cern.ch/root/html/TStyle.html#TStyle:SetLineStyleString
  gStyle->SetLineStyleString(12,"36 36"); //dashed
  gStyle->SetLineStyleString(13,"12 24"); //dotted
  gStyle->SetLineStyleString(14,"36 48 12 48"); //dash-dot
  gStyle->SetLineStyleString(15,"60 36 12 36 12 36 12 36"); //dash-triple-dot


  TH2D *hyield = getDetYieldEDist(zip,volts,eres,pres,nbins,0.0,xmax*1000,file,false);

  hyield->Draw("samecolz");
  //print up legend
  //leg->SetHeader(Form("%d GeV %s primaries ",energy,getFullPartName(part).c_str()));
  leg->SetFillColor(0);
  leg->SetLineWidth(2);
  leg->SetTextFont(42);
  leg->SetTextSize(.03);
  //leg->Draw("same");

  //output the file
  if(print){
    if(zoom)
      plotCanvas(c1,Form("figures/YRecoilDist%s_%s_zip%d_eres%d_pres%d_zoom",postfix.c_str(),datatype.c_str(),zip,(int)eres,(int)pres),ext);
    else
      plotCanvas(c1,Form("figures/YRecoilDist%s_%s_zip%d_eres%d_pres%d",postfix.c_str(),datatype.c_str(),zip,(int)eres,(int)pres),ext);
  }
}
void plotYieldFromLindhard(bool print=0,bool isprelim=true,double lindhard=0.159,string ext="eps",bool resave=false)
{
  
  //let's do some calculations to make the axis size ratio correct for some specified
  //ratio of x-size to y-size.  
  int xw=810,yw=-1;
  double r=1.0;
  double xr = 0.05;
  double xl = 0.15;
  double yt = 0.05;
  double yb = 0.12;
  double padsp=0.1;

  findCanvasSize(xw,yw,1,1,r,xr,xl,yt,yb,padsp);

  gROOT->SetStyle("Plain");
  //make a canvas
  TCanvas* c1 = (TCanvas*) gROOT->GetListOfCanvases()->FindObject("c1");
  if(c1){
    c1->Close();
    c1 = new TCanvas("c1","Yield vs. Recoil Energy Scale ",200,10,xw,yw);
  }
  else
    c1 = new TCanvas("c1","Yield vs. Recoil Energy Scale ",200,10,xw,yw);


  c1->cd();
  //log-log plot
  //c1->SetLogy();
  c1->SetLogx();
  c1->SetTickx();
  c1->SetTicky();
  c1->SetLeftMargin(xl);
  c1->SetBottomMargin(yb);
  c1->SetTopMargin(yt);
  c1->SetRightMargin(xr);
  //c1->SetGrid(1,1);
  TLegend *leg;
  leg = new TLegend(0.25,0.15,0.90,0.30);

  double px=20.0,py=0.09;
  //size of x-axis
  double xmin=1.0;
  double xmax=1000.0;
  double ymin=0.0;
  double ymax=0.20;

  gStyle->SetLineWidth(2);
  gStyle->SetHistLineWidth(1);
  gStyle->SetGridWidth(0);

  //make a frame for this histogram
  TH1* frame1;
  frame1 = makeFrame(xmin,xmax,ymin,ymax,10,"Recoil Energy (eV)","Ionization Yield",1.2,1.2);
  if(isprelim){
    TText *prelim = new TText(px,py,"preliminary");
    prelim->SetTextAngle(0);
    prelim->SetTextSize(0.1);
    prelim->SetTextColor(17);
    prelim->SetTextFont(42);
    frame1->GetListOfFunctions()->Add(prelim);
  }
  frame1->Draw();
  //TAxis *xaxis = frame1->GetXaxis();
  //xaxis->SetNoExponent(true);

  //make some new gstyles for thick lines
  //https://root.cern.ch/root/html/TStyle.html#TStyle:SetLineStyleString
  gStyle->SetLineStyleString(12,"36 36"); //dashed
  gStyle->SetLineStyleString(13,"12 24"); //dotted
  gStyle->SetLineStyleString(14,"36 48 12 48"); //dash-dot
  gStyle->SetLineStyleString(15,"60 36 12 36 12 36 12 36"); //dash-triple-dot

  Double_t perc=1.05;
  Double_t energies[3] = {672,562,407};
  Double_t cascade0[2] = {669.4,2.58};
  Double_t cascade1[3] = {553.8,5.47,2.58*perc};
  Double_t cascade2[4] = {382.8,15.72,5.47*perc,2.58*perc*perc};
  Double_t par[1];
  Double_t x[1];
  par[0]=0.159;
  TF1 *f = new TF1("lindhard",lindhard_k,0,1000,1);
  f->SetNpx(10000);
  f->SetParameter(0,0.159);
  f->SetLineColor(kBlack);
  f->Draw("same");
  leg->AddEntry(f,Form("Linhdard yield model (k=0.159)"),"l");
  //print up legend
  //leg->SetHeader(Form("%d GeV %s primaries ",energy,getFullPartName(part).c_str()));
  leg->SetFillColor(0);
  leg->SetLineWidth(2);
  leg->SetTextFont(42);
  leg->SetTextSize(.03);

  TGraph *g = new TGraph();
  x[0] = 672.0;
  g->SetPoint(0,672,lindhard_k(x,par));
  x[0] = 562.0;
  g->SetPoint(1,562,lindhard_k(x,par));
  x[0] = 407.0;
  g->SetPoint(2,407,lindhard_k(x,par));

  g->SetMarkerStyle(20);
  g->SetMarkerSize(1);
  g->SetMarkerColor(kBlack);
  g->Draw("p");

  x[0] = cascade0[0];
  TLine *l0_0 = new TLine(cascade0[0],ymin,cascade0[0],ymax);
  x[0] = cascade0[1];
  TLine *l0_1 = new TLine(cascade0[1],ymin,cascade0[1],ymax);
  l0_0->SetLineColor(kRed);
  l0_0->SetLineStyle(2);
  l0_0->SetLineWidth(3);
  l0_0->Draw("same");
  l0_1->SetLineColor(kRed);
  l0_1->SetLineStyle(2);
  l0_1->SetLineWidth(3);
  l0_1->Draw("same");

  x[0] = cascade1[0];
  TLine *l1_0 = new TLine(cascade1[0],ymin,cascade1[0],ymax);
  x[0] = cascade1[1];
  TLine *l1_1 = new TLine(cascade1[1],ymin,cascade1[1],ymax);
  x[0] = cascade1[2];
  TLine *l1_2 = new TLine(cascade1[2],ymin,cascade1[2],ymax);
  l1_0->SetLineColor(kBlue);
  l1_0->SetLineStyle(2);
  l1_0->SetLineWidth(3);
  l1_0->Draw("same");
  l1_1->SetLineColor(kBlue);
  l1_1->SetLineStyle(2);
  l1_1->SetLineWidth(3);
  l1_1->Draw("same");
  l1_2->SetLineColor(kBlue);
  l1_2->SetLineStyle(2);
  l1_2->SetLineWidth(3);
  l1_2->Draw("same");

  x[0] = cascade2[0];
  TLine *l2_0 = new TLine(cascade2[0],ymin,cascade2[0],ymax);
  x[0] = cascade2[1];
  TLine *l2_1 = new TLine(cascade2[1],ymin,cascade2[1],ymax);
  x[0] = cascade2[2];
  TLine *l2_2 = new TLine(cascade2[2],ymin,cascade2[2],ymax);
  x[0] = cascade2[3];
  TLine *l2_3 = new TLine(cascade2[3],ymin,cascade2[3],ymax);
  l2_0->SetLineColor(kGreen);
  l2_0->SetLineStyle(2);
  l2_0->SetLineWidth(3);
  l2_0->Draw("same");
  l2_1->SetLineColor(kGreen);
  l2_1->SetLineStyle(2);
  l2_1->SetLineWidth(3);
  l2_1->Draw("same");
  l2_2->SetLineColor(kGreen);
  l2_2->SetLineStyle(2);
  l2_2->SetLineWidth(3);
  l2_2->Draw("same");
  l2_3->SetLineColor(kGreen);
  l2_3->SetLineStyle(2);
  l2_3->SetLineWidth(3);
  l2_3->Draw("same");
  leg->AddEntry(l0_0,Form("2-gamma cascade"),"l");
  leg->AddEntry(l1_0,Form("3-gamma cascade"),"l");
  leg->AddEntry(l2_0,Form("4-gamma cascade"),"l");
  leg->Draw("same");
  //output the file
  if(print){
      plotCanvas(c1,Form("figures/YRecoilLindhardFunction"),ext);
  }
}

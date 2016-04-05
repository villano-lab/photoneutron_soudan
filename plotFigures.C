
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
void plotAllEDist(bool print=0,bool isprelim=true,int zip=1,string datatype="ybe", string ext="eps",bool resave=false)
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
    file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/ybe_trees_160405_all.root";
  }
  else if(datatype=="sbbe"){
    xmin=0.0;
    xmax=5.0;
    px=1.0;
    py=1e3;
    file="/data/chocula/villaa/PhotoN_SuperSim/ZipSum/sbbe_trees_160405_all.root";
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
  TH1D *hnrs = getDetNREDist(zip,100,xmin,xmax,true,file);
  TH1D *her = getDetEREDist(zip,100,xmin,xmax,true,file);
  TH1D *hmix = getDetEREDist(zip,100,xmin,xmax,false,file);
  hnr->SetLineColor(kBlue);
  hnrs->SetLineColor(kRed);
  TH1D *hsum = new TH1D();
  *hsum = *hnr;
  hsum->Add(hnrs);
  hsum->Add(her);
  hsum->Add(hmix);
  hsum->SetLineColor(kBlack);
  hsum->SetLineWidth(2);
  leg->AddEntry(hnr,Form("iZIP %d: NR multiples",zip),"l");
  leg->AddEntry(hnrs,Form("iZIP %d: NR singles",zip),"l");
  leg->AddEntry(her,Form("iZIP %d: ER all",zip),"l");
  leg->AddEntry(hmix,Form("iZIP %d: ER/NR mix",zip),"l");
  leg->AddEntry(hsum,Form("iZIP %d: all NR deposits",zip),"l");

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
    plotCanvas(c1,Form("figures/AllEDist_%s_zip%d",datatype.c_str(),zip),ext);
  }
}

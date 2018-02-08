
#include "RootUtil.C"
#include "plotFunctions.C"
#include "readPhotoNSuperSimTxt.C"
#include "drawTowers.C" 
#include "chainPhotoNSuperSim.C"

void plotPosShift(bool print=0,bool isprelim=true, string ext="eps",bool resave=false)
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
    c1 = new TCanvas("c1","Positions of Primaries",200,10,xw,yw);
  }
  else
    c1 = new TCanvas("c1","Positions of Primaries",200,10,xw,yw);


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
  leg = new TLegend(0.50,0.15,0.90,0.30);

  double px=-7,py=-5;
  //size of x-axis
  double xmin=-12;
  double xmax=12;
  double ymin=-12;
  double ymax=12;

  gStyle->SetLineWidth(2);
  gStyle->SetHistLineWidth(1);
  gStyle->SetGridWidth(0);

  //make a frame for this histogram
  TH1* frame1;
  frame1 = makeFrame(xmin,xmax,ymin,ymax,10,"north (-) / south (+) direction [cm]","west (-) / east (+) direction [cm]",1.2,0.8);
  if(isprelim){
    TText *prelim = new TText(px,py,"preliminary");
    prelim->SetTextAngle(90);
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

  //I don't think I usually have to do this but this function segfaults if
  //I run it twice with printing on something about overwriting the TH2Ds
  TH2D *h0 = (TH2D*) gROOT->FindObject("xy0");
  TH2D *h1 = (TH2D*) gROOT->FindObject("xy1");
  TH2D *h2 = (TH2D*) gROOT->FindObject("xy2");
  TH2D *h3 = (TH2D*) gROOT->FindObject("xy3");
  if(h0)
    delete h0;
  if(h1)
    delete h1;
  if(h2)
    delete h2;
  if(h3)
    delete h3;


  int nev;
  TTree *t = readPhotoNSuperSimTxtPrimaries(nev,-1,2);
  //TTree *t0 = readPhotoNSuperSimTxtPrimaries(nev,-1,0);
  TH2Poly *z = towerLayout();
  z->SetBinContent(2,100);
  z->SetBinContent(5,100);
  h0 = new TH2D("xy0","xy0",1000,-12,12,1000,-12,12);
  t->Draw("y*100:x*100>>xy0","","goff");

  h2 = new TH2D("xy2","xy2",1000,-12,12,1000,-12,12);
  t->Draw("y*100:x*100>>xy2","","goff");
  h3 = new TH2D("xy3","xy3",1000,-12,12,1000,-12,12);
  t->Draw("y*100:x*100>>xy3","shift==0","goff");
  h3->SetMarkerColor(kGreen);
  h3->SetMarkerStyle(20);

  h1 = new TH2D("xy1","xy1",1000,-12,12,1000,-12,12);
  t->Draw("y*100:x*100>>xy1","shift==0 && lshift==0","goff");
  h1->SetMarkerColor(kRed);
  h1->SetMarkerStyle(20);


  //leg->AddEntry(z,Form("iZIP towers"),"l");
  //leg->AddEntry(z,Form("iZIP towers"),"l");

  z->Draw("colz same");
  h0->Draw("same");
  h2->Draw("same");
  h3->Draw("same");
  h1->Draw("same");
  //print up legend
  //leg->SetHeader(Form("%d GeV %s primaries ",energy,getFullPartName(part).c_str()));
  //leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetLineWidth(2);
  leg->SetTextFont(42);
  leg->SetTextSize(.03);
  //leg->Draw("same");

  //output the file
  if(print){
    cout << c1 << endl;
    plotCanvas(c1,Form("figures/geomShiftLayout"),ext);
  }

}
void plotRatesLineShift(bool print=0,bool isprelim=true, string ext="eps",bool resave=false)
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
    c1 = new TCanvas("c1","Gamma Rates",200,10,xw,yw);
  }
  else
    c1 = new TCanvas("c1","Gamma Rates",200,10,xw,yw);


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
  leg = new TLegend(0.20,0.57,0.60,0.67);

  double px=-12.8996,py=2728;
  //size of x-axis
  double xmin=-14;
  double xmax=7.0;
  double ymin=10;
  double ymax=1e4;

  gStyle->SetLineWidth(2);
  gStyle->SetHistLineWidth(1);
  gStyle->SetGridWidth(0);

  //make a frame for this histogram
  TH1* frame1;
  frame1 = makeFrame(xmin,xmax,ymin,ymax,10,"along-bridge shift [cm]","rate [Hz/mCi]",1.2,0.8);
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

  //first try to find the info in a file
  TFile *f = new TFile("data/possysdata.root","UPDATE");
  string h14name = "it5z2_100"; //name and threshold
  string h4name = "it2z1_1000"; //name and threshold

  int nev;
  TChain *ch = chainPhotoNSuperSimPosSys(nev,-1,1,"88y","/data/chocula/villaa/PhotoN_SuperSim/possys");

  //the shift step is 0.86 cm
  double binwidth = (6.7/2.0/2.0/2.0)/10.0;
  double xend = 6.7 + (binwidth/2.0);
  double xstart = -13.4 - (binwidth/2.0);
  int nbins = (xend-xstart)/(binwidth);
  TH1D *h14 = new TH1D("shift_it5z2","shift_it5z2",nbins,xstart,xend);
  TH1D *h4 = new TH1D("shift_it2z1","shift_it2z1",nbins,xstart,xend);
  TH1D *hnorm = new TH1D("shift_norm","shift_norm",nbins,xstart,xend);
  bool gotnorm=false;

  TGraphErrors *g14;
  if(!f->GetListOfKeys()->FindObject(h14name.c_str()) || resave){
    ch->Draw("shift>>shift_it5z2","Sum$(allzips.Edep*(allzips.DetNum==14))>100.0","goff",ch->GetEntries());
    if(!gotnorm){
      ch->Draw("shift>>shift_norm","","goff",ch->GetEntries());
      gotnorm=true;
    }
    g14 = new TGraphErrors();
    int count=0;
    for(int i=0;i<h14->GetNbinsX();i++){
     
      //get data 
      double x,y,xerr,yerr;
      y=h14->GetBinContent(i+1);
      x=h14->GetBinCenter(i+1);
      yerr=sqrt(y);
      xerr=0;

      //get normalization 
      double xn,yn;
      yn=hnorm->GetBinContent(i+1);
      xn=hnorm->GetBinCenter(i+1);


      if(yn>0.0){
        g14->SetPoint(count,x,(y/yn)*37000000.0);  //norm to 1mCi
        g14->SetPointError(count,xerr,yerr*(37000000.0/yn));
        count++;
      }
    }
    g14->SetMarkerStyle(20);
    g14->SetMarkerColor(kRed);
    g14->SetName(h14name.c_str());
    g14->SetTitle(h14name.c_str());
  }
  else{
    g14 = (TGraphErrors*) f->Get(h14name.c_str());
  }

  TGraphErrors *g4;
  if(!f->GetListOfKeys()->FindObject(h4name.c_str()) || resave){
    ch->Draw("shift>>shift_it2z1","Sum$(allzips.Edep*(allzips.DetNum==4))>1000.0","goff",ch->GetEntries());
    if(!gotnorm){
      ch->Draw("shift>>shift_norm","","goff",ch->GetEntries());
      gotnorm=true;
    }
    g4 = new TGraphErrors();
    int count=0;
    for(int i=0;i<h4->GetNbinsX();i++){
      
      //get data 
      double x,y,xerr,yerr;
      y=h4->GetBinContent(i+1);
      x=h4->GetBinCenter(i+1);
      yerr=sqrt(y);
      xerr=0;

      //get normalization 
      double xn,yn;
      yn=hnorm->GetBinContent(i+1);
      xn=hnorm->GetBinCenter(i+1);

      if(yn>0.0){
        g4->SetPoint(i,x,(y/yn)*37000000.0); //norm to 1mCi
        g4->SetPointError(i,xerr,yerr*(37000000.0/yn));
        count++;
      }
    }
    g4->SetMarkerStyle(21);
    g4->SetMarkerColor(kBlue);
    g4->SetName(h4name.c_str());
    g4->SetTitle(h4name.c_str());
  }
  else{
    g4 = (TGraphErrors*) f->Get(h4name.c_str());
  }
  
  
  leg->AddEntry(g14,Form("IT5Z2 rate above 100 eV"),"p");
  leg->AddEntry(g4,Form("IT2Z1 rate above 1 keV"),"p");

  g14->Draw("p");
  g4->Draw("p");
  //print up legend
  //leg->SetHeader(Form("%d GeV %s primaries ",energy,getFullPartName(part).c_str()));
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  //leg->SetFillStyle(0);
  leg->SetLineWidth(2);
  leg->SetTextFont(42);
  leg->SetTextSize(.03);
  leg->Draw("same");

  //output the file
  if(print){
    cout << c1 << endl;
    plotCanvas(c1,Form("figures/rateFromShift"),ext);
  }

  f->cd();
  g14->Write("",TObject::kOverwrite);
  g4->Write("",TObject::kOverwrite);
  f->Close();
}
void plotRatesLineShift_Fix(bool print=0,bool isprelim=true, string ext="eps",bool resave=false)
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
    c1 = new TCanvas("c1","Gamma Rates",200,10,xw,yw);
  }
  else
    c1 = new TCanvas("c1","Gamma Rates",200,10,xw,yw);


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
  leg = new TLegend(0.20,0.57,0.60,0.67);

  double px=-12.8996,py=2728;
  //size of x-axis
  double xmin=-14;
  double xmax=7.0;
  double ymin=10;
  double ymax=1e4;

  gStyle->SetLineWidth(2);
  gStyle->SetHistLineWidth(1);
  gStyle->SetGridWidth(0);

  //make a frame for this histogram
  TH1* frame1;
  frame1 = makeFrame(xmin,xmax,ymin,ymax,10,"along-bridge shift [cm]","rate [Hz/mCi]",1.2,0.8);
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

  //first try to find the info in a file
  TFile *f = new TFile("data/possysdata_Fix.root","UPDATE");
  string h14name = "it5z2_100"; //name and threshold
  string h4name = "it2z1_1000"; //name and threshold

  int nev;
  TChain *ch = chainPhotoNSuperSimPosSys(nev,-1,4,"88y","/data/chocula/villaa/PhotoN_SuperSim/88yroot");

  //the shift step is 0.86 cm
  double binwidth = (6.7/2.0/2.0/2.0)/10.0;
  double xend = 6.7 + (binwidth/2.0);
  double xstart = -13.4 - (binwidth/2.0);
  int nbins = (xend-xstart)/(binwidth);
  TH1D *h14 = new TH1D("shift_it5z2","shift_it5z2",nbins,xstart,xend);
  TH1D *h4 = new TH1D("shift_it2z1","shift_it2z1",nbins,xstart,xend);
  TH1D *hnorm = new TH1D("shift_norm","shift_norm",nbins,xstart,xend);
  bool gotnorm=false;

  TGraphErrors *g14;
  if(!f->GetListOfKeys()->FindObject(h14name.c_str()) || resave){
    ch->Draw("shift>>shift_it5z2","Sum$(allzips.Edep*(allzips.DetNum==14))>100.0","goff",ch->GetEntries());
    if(!gotnorm){
      ch->Draw("shift>>shift_norm","","goff",ch->GetEntries());
      gotnorm=true;
    }
    g14 = new TGraphErrors();
    int count=0;
    for(int i=0;i<h14->GetNbinsX();i++){
     
      //get data 
      double x,y,xerr,yerr;
      y=h14->GetBinContent(i+1);
      x=h14->GetBinCenter(i+1);
      yerr=sqrt(y);
      xerr=0;

      //get normalization 
      double xn,yn;
      yn=hnorm->GetBinContent(i+1);
      xn=hnorm->GetBinCenter(i+1);


      if(yn>0.0){
        g14->SetPoint(count,x,(y/yn)*37000000.0);  //norm to 1mCi
        g14->SetPointError(count,xerr,yerr*(37000000.0/yn));
        count++;
      }
    }
    g14->SetMarkerStyle(20);
    g14->SetMarkerColor(kRed);
    g14->SetName(h14name.c_str());
    g14->SetTitle(h14name.c_str());
  }
  else{
    g14 = (TGraphErrors*) f->Get(h14name.c_str());
  }

  TGraphErrors *g4;
  if(!f->GetListOfKeys()->FindObject(h4name.c_str()) || resave){
    ch->Draw("shift>>shift_it2z1","Sum$(allzips.Edep*(allzips.DetNum==4))>1000.0","goff",ch->GetEntries());
    if(!gotnorm){
      ch->Draw("shift>>shift_norm","","goff",ch->GetEntries());
      gotnorm=true;
    }
    g4 = new TGraphErrors();
    int count=0;
    for(int i=0;i<h4->GetNbinsX();i++){
      
      //get data 
      double x,y,xerr,yerr;
      y=h4->GetBinContent(i+1);
      x=h4->GetBinCenter(i+1);
      yerr=sqrt(y);
      xerr=0;

      //get normalization 
      double xn,yn;
      yn=hnorm->GetBinContent(i+1);
      xn=hnorm->GetBinCenter(i+1);

      if(yn>0.0){
        g4->SetPoint(i,x,(y/yn)*37000000.0); //norm to 1mCi
        g4->SetPointError(i,xerr,yerr*(37000000.0/yn));
        count++;
      }
    }
    g4->SetMarkerStyle(21);
    g4->SetMarkerColor(kBlue);
    g4->SetName(h4name.c_str());
    g4->SetTitle(h4name.c_str());
  }
  else{
    g4 = (TGraphErrors*) f->Get(h4name.c_str());
  }
  
  
  leg->AddEntry(g14,Form("IT5Z2 rate above 100 eV"),"p");
  leg->AddEntry(g4,Form("IT2Z1 rate above 1 keV"),"p");

  g14->Draw("p");
  g4->Draw("p");
  //print up legend
  //leg->SetHeader(Form("%d GeV %s primaries ",energy,getFullPartName(part).c_str()));
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  //leg->SetFillStyle(0);
  leg->SetLineWidth(2);
  leg->SetTextFont(42);
  leg->SetTextSize(.03);
  leg->Draw("same");

  //output the file
  if(print){
    cout << c1 << endl;
    plotCanvas(c1,Form("figures/rateFromShift"),ext);
  }

  f->cd();
  g14->Write("",TObject::kOverwrite);
  g4->Write("",TObject::kOverwrite);
  f->Close();
}

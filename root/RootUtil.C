/*Here are a collection of root utilities you might find
useful
10/25/11 anv
*/


//plot a  canvas to .png or .eps
void plotCanvas(TCanvas *c1, string name="default",bool dopng=false)
{
    if(!dopng)
     c1->Print(Form("%s.eps",name.c_str()));
    else
     c1->Print(Form("%s.png",name.c_str()));

}
//make a frame for plotting histos or graphs
TH1 *makeFrame(double xmin,double xmax,double ymin,double ymax, int bins, string xtit, string ytit,double ytitoff=0.8, double xtitoff=1.0)
{
     TH1 *frame = new TH1F("frame","",bins,xmin,xmax);
     frame->SetMinimum(ymin);
     frame->SetMaximum(ymax);
     frame->SetDirectory(0);
     frame->SetStats(0);
     frame->GetXaxis()->SetTitle(xtit.c_str());
     frame->GetXaxis()->CenterTitle(true);
     frame->GetXaxis()->SetTickLength(0.02);
     frame->GetXaxis()->SetTitleFont(62);
     frame->GetXaxis()->SetLabelSize(0.03);
     frame->GetXaxis()->SetTitleOffset(xtitoff);
     frame->GetYaxis()->SetTitle(ytit.c_str());
     frame->GetYaxis()->CenterTitle(true);
     frame->GetYaxis()->SetLabelSize(0.03);
     frame->GetYaxis()->SetTitleFont(62);
     frame->GetYaxis()->SetTitleOffset(ytitoff);

     return frame;
}
//make a frame for plotting histos or graphs
void modFrame(TH1* frame,double xmin,double xmax,double ymin,double ymax, int bins, string xtit, string ytit,double ytitoff=0.8, double xtitoff=1.0)
{
     //TH1 *frame = new TH1F("frame","",bins,xmin,xmax);
     frame->SetTitle("");
     frame->SetStats(0);
     frame->SetMinimum(ymin);
     frame->SetMaximum(ymax);
     frame->GetXaxis()->SetTitle(xtit.c_str());
     frame->GetXaxis()->CenterTitle(true);
     frame->GetXaxis()->SetTickLength(0.02);
     frame->GetXaxis()->SetTitleFont(62);
     frame->GetXaxis()->SetLabelSize(0.03);
     frame->GetXaxis()->SetTitleOffset(xtitoff);
     frame->GetYaxis()->SetTitle(ytit.c_str());
     frame->GetYaxis()->CenterTitle(true);
     frame->GetYaxis()->SetLabelSize(0.03);
     frame->GetYaxis()->SetTitleFont(62);
     frame->GetYaxis()->SetTitleOffset(ytitoff);

}
TH1F *restrictDomain(TH1F *h,double xmin,double xmax)
{
  Int_t n = h->GetNbinsX();
 
  vector<double> binx;
  Int_t count=1;
  Int_t firstbin=-1;
  while(count < n+1){
    if(h->GetBinLowEdge(count)>xmin && (h->GetBinLowEdge(count)+h->GetBinWidth(count)) < xmax){
      binx.push_back(h->GetBinLowEdge(count));
      if(firstbin==-1)
        firstbin=count;
    }
    count++;
  }
  binx.push_back(h->GetBinLowEdge(firstbin+binx.size()-1)+h->GetBinWidth(firstbin+binx.size()-1));

  //convert vector<double> to an array
  Double_t *xbins = (Double_t*) malloc(binx.size()*sizeof(Double_t));

  for(int i=0;i<binx.size();i++){
     xbins[i]=binx[i];
  }

  TH1F *h1 = new TH1F("copy","copy",binx.size()-1,xbins);
  Int_t count=1;
  Int_t count2=1;
  while(count < n+1){
    if(h->GetBinLowEdge(count)>xmin && (h->GetBinLowEdge(count)+h->GetBinWidth(count)) < xmax){
      h1->SetBinContent(count2,h->GetBinContent(count));
      h1->SetBinError(count2,h->GetBinError(count));
      count2++;
    }
    count++;
  }
  h1->SetLineColor(h->GetLineColor());
  h1->SetName(h->GetName());
  h1->SetTitle(h->GetTitle());
  //h1->SetMarkerStyle(h->SetMarkerStyle());
  //h1->SetMarkerColor(h->SetMarkerColor());

  return h1;
}
TGraphErrors *getTGraphErrFromTH1F(TH1F *h1)
{
  //make the information into a TGraphErrors
  TGraphErrors *g = new TGraphErrors(h1->GetNbinsX());
  for(int i=0;i<h1->GetNbinsX();i++){
    g->SetPoint(i,h1->GetBinCenter(i+1),h1->GetBinContent(i+1));
    g->SetPointError(i,0,sqrt(h1->GetBinContent(i+1)*h1->GetBinWidth(i+1))/h1->GetBinWidth(i+1));
  }

  g->SetName("returnedgraph");
  return g;

}
Bool_t writeObjToFile(TObject *addobj, TString file="storedhists.root")
{


  //find the TList in the current setting
  if(addobj != NULL){

     cout << "Writing Object: " << addobj->GetName() << endl;
     //open the file
     TFile *f = new TFile(file,"UPDATE");

     //check for the Tlist in the file
     TList *l;
     if((l = (TList*) f->Get("myObjectList"))){
        cout << "Use Existing List" << endl;
        l->Add(addobj);
        l->Write("myObjectList",TObject::kSingleKey+TObject::kOverwrite);
     }
     else{
        cout << "Use NEW List" << endl;
        l = new TList();
	l->SetName("myObjectList");
        l->Add(addobj);
        l->Write("myObjectList",TObject::kSingleKey);
     }

     f->Close();

     return true;
  }

  return false;
}
TObject *getObjFromFile(TString name, TString file="storedhists.root")
{

   //open the file 
   TFile *f = new TFile(file,"READ");

   //if file doesn't exist just return null
   if(!f)
     return NULL;

   //check for the Tlist in the file
   TList *l;
   if((l = (TList*) f->Get("myObjectList"))){
      //cout << "Found Existing List" << endl;
      return l->FindObject(name);
   }

  return NULL;
}
void removeObjFromFile(TString name, TString file="storedhists.root")
{

   //open the file 
   TFile *f = new TFile(file,"UPDATE");

   //if file doesn't exist just return 
   if(!f)
     return;

   //check for the Tlist in the file
   TList *l;
   if((l = (TList*) f->Get("myObjectList"))){
      //cout << "Found Existing List" << endl;
      l->Remove(l->FindObject(name));
      l->Write("myObjectList",TObject::kSingleKey+TObject::kOverwrite);
   }

  return;
}
bool findCanvasSize(int &xw,int &yw, int row=1, int col=1,double r=1.0,double xr=0.05,double xl=0.15,double yt=0.05,double yb=0.12,double padsp=0.1)
{
  //check inputs
  if((yw<1 && xw<1) || (row<1 || col<1)){
    cerr << "ERROR (findCanvasSize): no dimension supplied for canvas size" << endl;
    return false;
  }

  //figure out which dimension is set (if input is negative dim is not set)
  bool xIsSet=true;
  if(xw<1)
    xIsSet=false; 

  //figure out which dimension is constraining
  bool yIsConstr=true;
  if(row<col)
    yIsConstr=false;

  
  //for the particular row and col settings get the margins
  double xmar=0.0,ymar=0.0;
  xmar = col*(xr+xl) + (col-1)*padsp;
  ymar = row*(yt+yb) + (row-1)*padsp;


  //get the availible screen size
  Int_t xs,ys;
  UInt_t w,h;
  gVirtualX->GetWindowSize(gClient->GetDefaultRoot()->GetId(),xs,ys,w,h);
  w = gClient->GetDisplayWidth();
  h = gClient->GetDisplayHeight();
  //cout << w << "  " << h << "  " << xs << "  " << ys << endl; 

  //reset the set dimension if is larger than screen dimension
  if(xIsSet){
    if(xw>(w-10)){
      xw = w-10;
    }
    //set the other dimension
    yw =(int)(((double)xw/r)*((1-xmar)/(1-ymar)));
  }
  else{
    if(yw>(h-10)){
      yw = h-10;
    }
    //set the other dimension
    xw = (int)(((double)yw*r)*((1-ymar)/(1-xmar)));
  }

  //reset the set dimension if is larger than screen dimension
  if(xIsSet){
    if(yw>(h-10)){
      yw = h-10;
    }
    //set the other dimension
    xw = (int)(((double)yw/r)*((1-ymar)/(1-xmar)));
  }
  else{
    if(xw>(w-10)){
      xw = w-10;
    }
    //set the other dimension
    yw =(int)(((double)xw*r)*((1-xmar)/(1-ymar)));
  }

  return true;

}

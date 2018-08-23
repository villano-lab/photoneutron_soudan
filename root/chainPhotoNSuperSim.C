/*some functions to chain specific types of data
from the simulation directory on the UMN system
this directory is as of writing /data/chocula/villaa/NeutReflect/SLACroot/
and contains the condensed root files produced from the slac data*/

#include <iomanip>
TChain *chainPhotoNSuperSim(int &nev,int n=-1,int datasetno=0,string source="ybe",string dir="/data/chocula/villaa/PhotoN_SuperSim/sbberoot",string treename="mcmerged")
{
   //get data set number string in hex
   string dataset="";
   if(datasetno>2){
     std::ostringstream oss;
     oss << std::hex << "0x" << std::setfill('0') << std::setw(4) << datasetno;
     dataset = oss.str(); 
   }

  //create a chain
  TChain *data = new TChain(treename.c_str(),treename.c_str());

  //make a command to use with a root pipe
  command = "ls "+dir+" |grep .root |grep "+dataset+"_ |grep "+source+" |grep _merge";
  TString files = gSystem->GetFromPipe(command.c_str());
  istringstream iss(files);

  //get the files from the string
  string filename;

  //make a counter
  int count=0;

  while(!iss.eof()){
    iss >> filename;

    if(count>=n && n>0)
      break;

    if(n<0 || count<n){
      string fullfilename = dir+"/"+filename;
      cout << fullfilename << endl;
      data->Add(fullfilename.c_str());
    }
    count++;
  }

  
  return data;

}
TChain *chainPhotoNSuperSim_wOffset(int &nev,int n=-1,int off=0,int datasetno=0,string source="ybe",string dir="/data/chocula/villaa/PhotoN_SuperSim/sbberoot",string treename="mcmerged")
{
   //get data set number string in hex
   string dataset="";
   if(datasetno>2){
     std::ostringstream oss;
     oss << std::hex << "0x" << std::setfill('0') << std::setw(4) << datasetno;
     dataset = oss.str(); 
   }

  //create a chain
  TChain *data = new TChain(treename.c_str(),treename.c_str());

  //make a command to use with a root pipe
  command = "ls "+dir+" |grep .root |grep "+dataset+"_ |grep "+source+" |grep _merge";
  TString files = gSystem->GetFromPipe(command.c_str());
  istringstream iss(files);

  //get the files from the string
  string filename;

  //make a counter
  int count=0;

  while(!iss.eof()){
    iss >> filename;

    //cout << count << "\t" << off << endl;
    if(count<off){
      count++;
      continue;
    }

    if(((count-off)>=n)&&(n>0))
      break;

    if(n<0 || (count-off)<n){
      string fullfilename = dir+"/"+filename;
      cout << fullfilename << endl;
      data->Add(fullfilename.c_str());
    }
    count++;
  }

  
  return data;

}
TChain *chainPhotoNSuperSimPosSys(int &nev,int n=-1,int datasetno=0,string source="88y",string dir="/data/chocula/villaa/PhotoN_SuperSim/88yroot",string treename="mcmerged")
{
   //get data set number string in hex
   string dataset="";
   std::ostringstream oss;
   oss << std::hex << "0x" << std::setfill('0') << std::setw(4) << datasetno;
   dataset = oss.str(); 

  //create a chain
  TChain *data = new TChain(treename.c_str(),treename.c_str());
  TChain *shiftch = new TChain("metaData","metaData");


  //make a command to use with a root pipe
  command = "ls "+dir+" |grep .root |grep "+dataset+"_ |grep "+source+" |grep _merge";
  TString files = gSystem->GetFromPipe(command.c_str());
  istringstream iss(files);

  //get the files from the string
  string filename;

  //make a counter
  int count=0;

  while(!iss.eof()){
    iss >> filename;


    if(n<0 || count<n){
      string fullfilename = dir+"/"+filename;
      bool isok = insertMetaData(fullfilename);
      if(!isok) continue;
      cout << fullfilename << endl;
      data->Add(fullfilename.c_str());
      shiftch->Add(fullfilename.c_str());
    }
    count++;
  }
 
  data->AddFriend(shiftch);
  return data;

}
bool insertMetaData(string filename,bool force=false)
{
  //read the root file, find a Tree called metaData, overwrite it with new meta data
  //open the file and get the old tree
  TFile *f = new TFile(filename.c_str(),"UPDATE");

  bool havetree = (bool)f->GetListOfKeys()->FindObject("metaData");

  if(havetree && !force) return true;

  TTree *oldt = f->Get("mcmerged");
  if(!oldt) return false;
  //make a tree
  TTree *t = new TTree("metaData","metaData");

  double shift,lshift;
  t->Branch("shift",&shift,"shift/D");
  t->Branch("lshift",&lshift,"lshift/D");

  //get meta data
  //extract the shift from the filename
  int p0 = filename.find("shift");
  int p1 = filename.find("_lshift");
  int p2 = filename.find("_p");

  string strshift;
  string strlshift;
  if(p1!=string::npos){
    strshift = filename.substr(p0+5,p1-p0-5);
    strlshift = filename.substr(p1+7,p2-p1-7);
  }
  else{
    strshift = filename.substr(p0+5,p2-p0-5);
    strlshift = "-99999";
  }
  //strshift = "134_0556";
  cout << strshift << endl;
  cout << strlshift << endl;

  int u = strshift.find("_");
  string strshift_dec;
  if(u!=string::npos){
    strshift_dec=strshift.substr(0,u);
    strshift_dec+=".";
    strshift_dec+=strshift.substr(u+1,strshift.size()-u);
  }
  else
    strshift_dec=strshift;

  u = strlshift.find("_");
  string strlshift_dec;
  if(u!=string::npos){
    strlshift_dec=strlshift.substr(0,u);
    strlshift_dec+=".";
    strlshift_dec+=strlshift.substr(u+1,strlshift.size()-u);
  }
  else
    strlshift_dec=strlshift;

  //change to double
  istringstream streamshift(strshift_dec);
  streamshift >> shift;
  istringstream streamlshift(strlshift_dec);
  streamlshift >> lshift;

  //echo the computed shift
  cout << "The computed shift is: " << shift << endl;
  cout << "The computed lshift is: " << lshift << endl;

  //add this to the tree
  for(int i=0;i<oldt->GetEntries();i++) t->Fill();



  //write it back to the file
  t->Write("",TObject::kOverwrite);
  f->Close();

  return true;
}
bool insertMetaData_Light(string filename,bool force=false)
{
  //try to save on space and not assume I have one root tree entry for every event

  //read the root file, find a Tree called metaData, overwrite it with new meta data
  //open the file and get the old tree
  TFile *f = new TFile(filename.c_str(),"UPDATE");

  bool havetree = (bool)f->GetListOfKeys()->FindObject("metaData");

  if(havetree && !force) return true;

  TTree *oldt = f->Get("mcmerged");
  if(!oldt) return false;
  //make a tree
  TTree *t = new TTree("metaData","metaData");

  double shift,lshift;
  t->Branch("shift",&shift,"shift/D");
  t->Branch("lshift",&lshift,"lshift/D");

  //get meta data
  //extract the shift from the filename
  int p0 = filename.find("shift");
  int p1 = filename.find("_lshift");
  int p2 = filename.find("_p");

  string strshift;
  string strlshift;
  if(p1!=string::npos){
    strshift = filename.substr(p0+5,p1-p0-5);
    strlshift = filename.substr(p1+7,p2-p1-7);
  }
  else{
    strshift = filename.substr(p0+5,p2-p0-5);
    strlshift = "-99999";
  }
  //strshift = "134_0556";
  cout << strshift << endl;
  cout << strlshift << endl;

  int u = strshift.find("_");
  string strshift_dec;
  if(u!=string::npos){
    strshift_dec=strshift.substr(0,u);
    strshift_dec+=".";
    strshift_dec+=strshift.substr(u+1,strshift.size()-u);
  }
  else
    strshift_dec=strshift;

  u = strlshift.find("_");
  string strlshift_dec;
  if(u!=string::npos){
    strlshift_dec=strlshift.substr(0,u);
    strlshift_dec+=".";
    strlshift_dec+=strlshift.substr(u+1,strlshift.size()-u);
  }
  else
    strlshift_dec=strlshift;

  //change to double
  istringstream streamshift(strshift_dec);
  streamshift >> shift;
  istringstream streamlshift(strlshift_dec);
  streamlshift >> lshift;

  //echo the computed shift
  cout << "The computed shift is: " << shift << endl;
  cout << "The computed lshift is: " << lshift << endl;

  //add this to the tree
  for(int i=0;i<oldt->GetEntries();i++) t->Fill();



  //write it back to the file
  t->Write("",TObject::kOverwrite);
  f->Close();

  return true;
}

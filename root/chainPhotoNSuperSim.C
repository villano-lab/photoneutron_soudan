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
    if(n<0 || count<n){
      string fullfilename = dir+"/"+filename;
      data->Add(fullfilename.c_str());
    }
    count++;
  }
  
  return data;

}



#include <iomanip>
TTree *readPhotoNSuperSimTxtPrimaries(int &nev,int n=-1,int datasetno=0,string source="88y",string dir="/data/chocula/villaa/PhotoN_SuperSim/88y",string treename="mcprimaries")
{
   //get data set number string in hex
   string dataset="";
   std::ostringstream oss;
   oss << std::hex << "0x" << std::setfill('0') << std::setw(4) << datasetno;
   dataset = oss.str(); 

  //create a chain
  TTree *data = new TTree(treename.c_str(),treename.c_str());

  //set up the variables
  double E,x,y,z,px,py,pz,D,t,shift,lshift;
  int event,type;
  data->Branch("E",&E,"E/D");
  data->Branch("x",&x,"x/D");
  data->Branch("y",&y,"y/D");
  data->Branch("z",&z,"z/D");
  data->Branch("px",&px,"px/D");
  data->Branch("py",&py,"py/D");
  data->Branch("pz",&pz,"pz/D");
  data->Branch("D",&D,"D/D");
  data->Branch("t",&t,"t/D");
  data->Branch("shift",&shift,"shift/D");
  data->Branch("lshift",&lshift,"lshift/D");
  data->Branch("event",&event,"event/I");
  data->Branch("type",&type,"type/I");

  //make a command to use with a root pipe
  command = "ls "+dir+" |grep Primary.txt |grep "+dataset+"_ |grep "+source;
  TString files = gSystem->GetFromPipe(command.c_str());
  istringstream iss(files);

  //get the files from the string
  string filename;

  //make a counter
  int count=0;

  while(!iss.eof()){
    iss >> filename;
    cout << filename << endl;

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

    //fill the tree
    ifstream streamfile((dir+"/"+filename).c_str(),ios::in);
    cout << (dir+"/"+filename) << endl;
    int filecount=0;
    while(!streamfile.eof()){
      if(filecount>2){
        streamfile >> event >> type >> E >> px >> py >> pz >> x >> y >> z >> D >> t;
	data->Fill();
	//cout << filecount << "\t" << event << "\t" << type << "\t" << E << "\t" << px << "\t" << py << "\t" << pz << "\t" << x << "\t" << y << "\t" << z << "\t" << t << endl;
      }
      else{
	string junk;
	getline(streamfile,junk);
      }
      filecount++;
    }
    streamfile.close();

    count++;
  }
  
  return data;

}

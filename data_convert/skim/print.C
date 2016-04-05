
void print(string filename){

	TFile *f = new TFile(filename.c_str());
	TTree *t = f->Get("mcmerged");
	t->Print();

	return;
}

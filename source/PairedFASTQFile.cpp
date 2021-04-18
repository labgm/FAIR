#include "SingleFASTQFile.cpp"
 
class PairedFASTQFile
{
	string file;
	SingleFASTQFile forward, reverse, identAdapter, adaptersVec;
	PairedFASTQ pairedSequence;
	pair<string, string> adapters;

	string textOutputIdentify;

public:
	bool openFASTQInputFile(string forward, string reverse, int quality);
	bool openFASTQOutputFile(string file, string file2);
	bool hasNext();
	PairedFASTQ getNext();
	pair<string, string> identifyAdapters();
	void trim(int minQuality, int sizeQualityWindow, bool trimNFlank, int maxN);
	void removeAdapters(bool onlyRemove, string adapter1, string adapter2, int mismatchMax, string adapterInvert_f, string adapterInvert_r, double mismatchRight, double five_dr, bool rre);
	void write();
	void closeOutput(string typeOperation);

	bool hasNextSearchAdapters();

	int getReadAffected();

	int returnNAffectedBases_f();
	int returnNAffectedBases_r();
	int getSequenceN();
	int getSequenceN_f();
	int getSequenceN_r();

};

bool PairedFASTQFile::openFASTQInputFile(string forward, string reverse, int quality)
{
	if (this->forward.openFASTQInput(forward, quality) && this->reverse.openFASTQInput(reverse, quality))
	{
		return true;
	}

	cerr << "Failed To Open Forward & Reverse Files." << endl;
	return false;
}


bool PairedFASTQFile::openFASTQOutputFile(string file, string file2)
{
	this->file = file;

	if (this->forward.openFASTQOutput(file) && this->reverse.openFASTQOutput(file2) == true)
	{
		return true;
	}

	return false;
}

bool PairedFASTQFile::hasNext()
{
	return (forward.hasNext() && reverse.hasNext());
}

bool PairedFASTQFile::hasNextSearchAdapters()
{
	if(forward.hasNextSearchAdapters("forward") && reverse.hasNextSearchAdapters("reverse"))
	{
		return true;
	}

	forward.writeOnlyIdentifyHeader("forward");
	reverse.writeOnlyIdentifyHeader("reverse");

	for (int i = 0; i < forward.getAdaptersVec().size(); ++i)
	{
		cerr << "Forward: " << forward.getAdaptersVec()[i] << "\t" << forward.getAdaptersVecQuant()[i] << endl;
		textOutputIdentify = forward.getAdaptersVec()[i] + "\t" + std::to_string(forward.getAdaptersVecQuant()[i]);
		forward.writeOnlyIdentify(textOutputIdentify);

	}
	for (int i = 0; i < reverse.getAdaptersVec().size(); ++i)
	{
		cerr << "Reverse: " << reverse.getAdaptersVec()[i] << "\t" << reverse.getAdaptersVecQuant()[i] << endl;
		textOutputIdentify = reverse.getAdaptersVec()[i] + "\t" + std::to_string(reverse.getAdaptersVecQuant()[i]);
		reverse.writeOnlyIdentify(textOutputIdentify);
	}

	return false;
}

PairedFASTQ PairedFASTQFile::getNext()
{
	pairedSequence.setPair(forward.getNext(), reverse.getNext());
	return pairedSequence;
}

pair<string, string> PairedFASTQFile::identifyAdapters()
{
	adapters.first = forward.identifyAdapter();
	adapters.second = reverse.identifyAdapter();
	return adapters;
}

void PairedFASTQFile::trim(int minQuality, int sizeQualityWindow, bool trimNFlank, int maxN)
{
	forward.trim(minQuality, sizeQualityWindow, trimNFlank, maxN);
	reverse.trim(minQuality, sizeQualityWindow, trimNFlank, maxN);
}

void PairedFASTQFile::removeAdapters(bool onlyRemove, string adapter1, string adapter2, int mismatchMax, string adapterInvert_f, string adapterInvert_r, double mismatchRight, double five_dr, bool rre)
{
	forward.removeAdapter(onlyRemove, adapter1, mismatchMax, adapterInvert_f, mismatchRight, five_dr, rre);
	reverse.removeAdapter(onlyRemove, adapter2, mismatchMax, adapterInvert_r, mismatchRight, five_dr, rre);
}

void PairedFASTQFile::write()
{
	forward.write();
	reverse.write();
}

void PairedFASTQFile::closeOutput(string typeOperation)
{
	forward.closeOutput(typeOperation);
	reverse.closeOutput(typeOperation);
}

// Retorna 0 se o corte de qualidade não afetou nenhuma read,
// 1 caso afetou apenas forward,
// 2 caso afetou apenas reverse,
// e 3 caso afetou as duas leituras
int PairedFASTQFile::getReadAffected()
{
	bool f = forward.getReadAffected();
	bool r = reverse.getReadAffected();
	int sum = 0;
	if (f == true) sum += 1;
	if (r == true) sum += 2;
	return sum;
}

// Retorna quantidade de bases cortadas por qualidade
int PairedFASTQFile::returnNAffectedBases_f()
{
	int f = forward.returnNAffectedBases();
	return f;
}

int PairedFASTQFile::returnNAffectedBases_r()
{
	int r = reverse.returnNAffectedBases();
	return r;
}

// Retorna somatorio de bases das duas sequencias (forward + reverse)
int PairedFASTQFile::getSequenceN()
{
	int n_seq_f = forward.getSequenceN();
	int n_seq_r = reverse.getSequenceN();
	return (n_seq_f+n_seq_r);
}

// Retorna somatorio de bases das duas sequencias (forward e reverse)
int PairedFASTQFile::getSequenceN_f()
{
	int n_seq_f = forward.getSequenceN();
	return (n_seq_f);
}
int PairedFASTQFile::getSequenceN_r()
{
	int n_seq_r = reverse.getSequenceN();
	return (n_seq_r);
}
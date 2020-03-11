#include "SingleFASTQFile.cpp"

#include <pthread.h>
#include <thread>

class PairedFASTQFile
{
	string file;
	SingleFASTQFile forward, reverse;
	PairedFASTQ pairedSequence;
	pair<string, string> adapters;

public:
	// bool openFASTQInputFile(string forward, string reverse, int quality);
	// bool openFASTQOutputFile(string file, string file2);
	// bool hasNext();
	PairedFASTQ getNext();
	pair<string, string> identifyAdapters();
	void trim(int minQuality, int minSequenceLength);
	// void removeAdapters(bool onlyRemove, string adapter1, string adapter2);
	// void write();
	// void closeOutput();

	bool openFASTQInputFile_forward(string forward, int quality);
	bool openFASTQInputFile_reverse(string reverse, int quality);
	bool openFASTQOutputFile_forward(string file);
	bool openFASTQOutputFile_reverse(string file2);
	bool hasNext_forward();
	bool hasNext_reverse();
	void trim_forward(int minQuality, int minSequenceLength);
	void trim_reverse(int minQuality, int minSequenceLength);
	void removeAdapters_forward(bool onlyRemove, string adapter1);
	void removeAdapters_reverse(bool onlyRemove, string adapter2);
	void write_forward();
	void write_reverse();
	void closeOutput_forward();
	void closeOutput_reverse();

};

bool PairedFASTQFile::openFASTQInputFile_forward(string forward, int quality)
{
	if (this->forward.openFASTQInput(forward, quality))
	{
		return true;
	}

	cerr << "Failed To Open Forward & Reverse Files." << endl;
	return false;
}
bool PairedFASTQFile::openFASTQInputFile_reverse(string reverse, int quality)
{
	if (this->reverse.openFASTQInput(reverse, quality))
	{
		return true;
	}

	cerr << "Failed To Open Forward & Reverse Files." << endl;
	return false;
}

bool PairedFASTQFile::openFASTQOutputFile_forward(string file)
{
	this->file = file;

	if (this->forward.openFASTQOutput(file) == true)
	{
		return true;
	}

	return false;
}
bool PairedFASTQFile::openFASTQOutputFile_reverse(string file2)
{
	// this->file = file;

	if (this->reverse.openFASTQOutput(file2) == true)
	{
		return true;
	}

	return false;
}

bool PairedFASTQFile::hasNext_forward()
{
	return (forward.hasNext());
}
bool PairedFASTQFile::hasNext_reverse()
{
	return (reverse.hasNext());
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

void PairedFASTQFile::trim_forward(int minQuality, int minSequenceLength)
{
	forward.trim(minQuality, 0);
}
void PairedFASTQFile::trim_reverse(int minQuality, int minSequenceLength)
{
	reverse.trim(minQuality, 0);
}

void PairedFASTQFile::removeAdapters_forward(bool onlyRemove, string adapter1)
{
	forward.removeAdapter(onlyRemove, adapter1);
}
void PairedFASTQFile::removeAdapters_reverse(bool onlyRemove, string adapter2)
{
	reverse.removeAdapter(onlyRemove, adapter2);
}

void PairedFASTQFile::write_forward()
{
	forward.write();
}
void PairedFASTQFile::write_reverse()
{
	reverse.write();
}

void PairedFASTQFile::closeOutput_forward()
{
	forward.closeOutput();
}
void PairedFASTQFile::closeOutput_reverse()
{
	reverse.closeOutput();
}



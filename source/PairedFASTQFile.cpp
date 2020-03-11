// *#
#include <thread>
#include <pthread.h>

#define THREADS_NUM 4


// #include <unistd.h>  //Header file for sleep(). man 3 sleep for details. 
// #include <pthread.h> 

#include "SingleFASTQFile.cpp"

class PairedFASTQFile
{
	string file;
	SingleFASTQFile forward, reverse;
	PairedFASTQ pairedSequence;
	pair<string, string> adapters;

public:
	bool openFASTQInputFile(string forward, string reverse, int quality);
	bool openFASTQOutputFile(string file, string file2);
	bool hasNext();
	PairedFASTQ getNext();
	pair<string, string> identifyAdapters();
	void trim(int minQuality, int minSequenceLength);
	void removeAdapters(bool onlyRemove, string adapter1, string adapter2);
	void write();
	void closeOutput();
	void removeForward(bool onlyRemove, string adapter);
	void removeReverse(bool onlyRemove, string adapter);
	// *#

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

void PairedFASTQFile::trim(int minQuality, int minSequenceLength)
{
	forward.trim(minQuality, 0);
	reverse.trim(minQuality, 0);
}

// *#
void PairedFASTQFile::removeForward(bool onlyRemove, string adapter)
{
	// cerr << "Removendo Forward" << endl;
	forward.removeAdapter(onlyRemove, adapter);
	// cerr << "Forward FOI" << endl;
}
void PairedFASTQFile::removeReverse(bool onlyRemove, string adapter)
{
	// cerr << "Removendo Reverse" << endl;
	reverse.removeAdapter(onlyRemove, adapter);
	// cerr << "Reverse FOI" << endl;
}

void PairedFASTQFile::removeAdapters(bool onlyRemove, string adapter1, string adapter2)
{

	// PairedFASTQFile pff;
	// thread t1 {&PairedFASTQFile::removeForward, &pff, onlyRemove, adapter1};
	// t1.join();
	// thread t2 {&PairedFASTQFile::removeReverse, &pff, onlyRemove, adapter2};
	// t2.join();

	forward.removeAdapter(onlyRemove, adapter1);
	reverse.removeAdapter(onlyRemove, adapter2);

}

void PairedFASTQFile::write()
{
	forward.write();
	reverse.write();
}

void PairedFASTQFile::closeOutput()
{
	forward.closeOutput();
	reverse.closeOutput();
}



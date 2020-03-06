// *#
#include <thread>


#include <unistd.h>  //Header file for sleep(). man 3 sleep for details. 
#include <pthread.h> 

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
	// *#
	void operator()(bool onlyRemove, string adapter1){
			forward.removeAdapter(onlyRemove, adapter1);
	}
	void operator2()(bool onlyRemove, string adapter2){
			reverse.removeAdapter(onlyRemove, adapter2);
	}
	// void ThreadTestReverse()(string adapter2, bool onlyRemove){
	// 		reverse.removeAdapter(onlyRemove, adapter1);
	// }

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


void accumulator_function2(int endIndex)
{
    for (unsigned int i = 0; i < endIndex; ++i)
    {
        cerr << i << endl;
    }
}

// *#
void PairedFASTQFile::removeAdapters(bool onlyRemove, string adapter1, string adapter2)
{
	// forward.removeAdapter(onlyRemove, adapter1);
	// reverse.removeAdapter(onlyRemove, adapter2);
	// std::thread ThreadTestFoward(onlyRemove, adapter1);
	// std::thread ThreadTestReverse(onlyRemove, adapter2);
	// std::thread t1(forward.removeAdapter, onlyRemove, adapter1);


	// std::thread t1 ThreadTestFoward(onlyRemove, adapter1);
	// ThreadTestReverse(onlyRemove, adapter2);


	// 
	// thread t1 (std::ThreadTestFoward, onlyRemove, adapter1);
	// int a  = 1;
	// std::thread t1(accumulator_function2, a);
	// ThreadTestReverse.join();
	thread t1(PairedFASTQFile(), onlyRemove, adapter1);
	thread t2(PairedFASTQFile(), onlyRemove, adapter2); 
	// thread t2(PairedFASTQFile(), adapter2, onlyRemove); 
	t1.join();
	t2.join();

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


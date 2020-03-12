#include "PairedFASTQ.cpp"

class SingleFASTQFile
{
	SingleFASTQ sequence;
	string file, adapter;
	ofstream fout;
	ifstream fin;
	int quality; // 33 or 64

public:
	bool openFASTQInput(string file, int quality);
	bool openFASTQOutput(string file);
	bool hasNext();
	SingleFASTQ getNext();
	string identifyAdapter();
	void identifyQuality();
	void trim(int minQuality, int minSequenceLength);
	void removeAdapter(bool onlyRemove, string adapter);
	void write();
	void closeOutput();
	void closeOutput_paired(int typeRead);

	void identifyQuality_forward();
	void identifyQuality_reverse();

	bool openFASTQInput_forward(string file, int quality);
	bool openFASTQInput_reverse(string file, int quality);

};

bool SingleFASTQFile::openFASTQInput(string file, int quality)
{
	this->file = file;

	if (quality == 0)
	{
		identifyQuality();
	}
	else
	{
		this->quality = quality;
	}

	fin.open(file);
	if (fin.is_open())
	{
		return true;
	}

	cerr << "Failed to Open Input File" << file << endl;
	return false;
}

bool SingleFASTQFile::openFASTQInput_forward(string file, int quality)
{
	this->file = file;

	if (quality == 0)
	{
		identifyQuality_forward();
	}
	else
	{
		this->quality = quality;
	}

	fin.open(file);
	if (fin.is_open())
	{
		return true;
	}

	cerr << "Failed to Open Input File" << file << endl;
	return false;
}
bool SingleFASTQFile::openFASTQInput_reverse(string file, int quality)
{
	this->file = file;

	if (quality == 0)
	{
		identifyQuality_reverse();
	}
	else
	{
		this->quality = quality;
	}

	fin.open(file);
	if (fin.is_open())
	{
		return true;
	}

	cerr << "Failed to Open Input File" << file << endl;
	return false;
}

bool SingleFASTQFile::openFASTQOutput(string file)
{
	fout.open(file);
	if (fout.is_open())
	{
		return true;
	}

	cerr << "Failed to Open Output File" << file << endl;
	return false;
}

bool SingleFASTQFile::hasNext()
{
	string lines[4];

	for (int i = 0; i < 4; i++)
		if (!getline(fin, lines[i]))
			return false;

	sequence.setIdentifier(lines[0]);
	sequence.setSequence(lines[1]);
	sequence.setPlaceHolder(lines[2]);
	sequence.setQuality(lines[3]);

	return true;
}

SingleFASTQ SingleFASTQFile::getNext()
{
	return sequence;
}

string SingleFASTQFile::identifyAdapter()
{
	// TODO
	return adapter;
}

void SingleFASTQFile::identifyQuality()
{

	cerr << "NOT" << endl;

	cerr << endl
		 << "Phred Quality Offset of " << file << ":" << endl;

	string command = "sed -n '2p' " + file + " > seq_sample.fastq";

	system(command.c_str());
	system("python3 source/bin/identify-phred.py seq_sample.fastq");

	ifstream fef("qual.txt");
	if (fef >> quality)
	{
		system("rm qual.txt && rm seq_sample.fastq");
		fef.close();
	}
	cerr << endl;
}

void SingleFASTQFile::identifyQuality_forward()
{

	cerr << endl
		 << "Phred Quality Offset of " << file << ":" << endl;

	string command = "sed -n '2p' " + file + " > seq_sample_forward.fastq";

	system(command.c_str());
	system("python3 source/bin/identify-phred.py seq_sample_forward.fastq");

	ifstream fef("qual.txt");
	if (fef >> quality)
	{
		system("rm qual.txt && rm seq_sample_forward.fastq");
		fef.close();
	}
	cerr << endl;
}
void SingleFASTQFile::identifyQuality_reverse()
{

	cerr << endl
		 << "Phred Quality Offset of " << file << ":" << endl;

	string command = "sed -n '2p' " + file + " > seq_sample_reverse.fastq";

	system(command.c_str());
	system("python3 source/bin/identify-phred.py seq_sample_reverse.fastq");

	ifstream fef("qual.txt");
	if (fef >> quality)
	{
		system("rm qual.txt && rm seq_sample_reverse.fastq");
		// system("rm seq_sample_reverse.fastq");
		fef.close();
	}
	cerr << endl;
}

void SingleFASTQFile::trim(int minQuality, int minSequenceLength)
{
	sequence.trim(quality, minQuality, minSequenceLength);
}

void SingleFASTQFile::removeAdapter(bool onlyRemove, string adapter)
{

	if (onlyRemove) // Adapter as Parameter
	{
		this->adapter = adapter;
	}
	else // Need to Identify
	{
		adapter = identifyAdapter();
	}

	// cerr << "Mark 1" << endl;

	sequence.erase(adapter);

	// cerr << "Mark 2" << endl;

	// int number_of_ocurrences = 0;

	// for (int i = newSequence.find(adapter, 0); i != string::npos; i = newSequence.find(adapter, i)) {
	// 	cerr <<"i: " << i << endl << "Number Occ: " << number_of_ocurrences << endl;
	// 	index.push_back(i);
	// 	number_of_ocurrences++;
	// 	i++;

	// }
}


void SingleFASTQFile::write()
{
	fout << sequence.getIdentifier() << "\n";
	fout << sequence.getSequence() << "\n";
	fout << sequence.getPlaceHolder() << "\n";
	fout << sequence.getQuality() << "\n";
}

void SingleFASTQFile::closeOutput()
{
	cerr << "Number of Occurrences: " << sequence.getOccurrences() << endl;
	fin.close();
	fout.close();
}

void SingleFASTQFile::closeOutput_paired(int typeRead)
{
	if(typeRead == 1)
	cerr << "Number of Occurrences (Forward): " << sequence.getOccurrences() << endl;
	else
	cerr << "Number of Occurrences (Reverse): " << sequence.getOccurrences() << endl;

	fin.close();
	fout.close();
}



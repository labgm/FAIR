//#include "algos/sbndmq4.cpp"
#include "algos/shiftand.cpp"

// #include "algos/shiftand_filtragem.cpp"
// #include "algos/myers.cpp"
// #include "algos/myers_minimized.cpp"

// #include "algos/myers_otimized.cpp"

class SingleFASTQ
{
    int occurrences = 0;
	vector<int> int_quality;
	string id, seq, pholder, qual;
	friend ostream &operator<<(ostream &os, const SingleFASTQ &single);

public:
	void setIdentifier(string id);
	string getIdentifier();
	void setSequence(string seq);
	string getSequence();
	void setPlaceHolder(string pholder);
	string getPlaceHolder();
	void setQuality(string qual);
	string getQuality();
	void convertQualToInteger(int qual_score);
	int getOccurrences();
	void erase(string adapter);
	void trim(int qual_score, int minQuality, int minSequenceLength);
};

void SingleFASTQ::setIdentifier(string id)
{
	this->id = id;
}
string SingleFASTQ::getIdentifier()
{
	return id;
}

void SingleFASTQ::setSequence(string seq)
{
	this->seq = seq;
}
string SingleFASTQ::getSequence()
{
	return seq;
}

void SingleFASTQ::setPlaceHolder(string pholder)
{
	this->pholder = pholder;
}
string SingleFASTQ::getPlaceHolder()
{
	return pholder;
}

void SingleFASTQ::setQuality(string qual)
{
	this->qual = qual;
}
string SingleFASTQ::getQuality()
{
	return qual;
}

void SingleFASTQ::convertQualToInteger(int qual_score)
{
	// Base 33 : Sanger, Illumina 1.8+.
	// Base 64 : Solexa, Illumina-1.3, Illumina-1.5.
	int_quality.clear();

	for (int i = 0; i < qual.length(); i++)
		int_quality.push_back(static_cast<int>(qual[i]) - qual_score);

}

int SingleFASTQ::getOccurrences()
{
	return occurrences;
}


void SingleFASTQ::erase(string adapter)
{
	vector<int> index;

	char seq_c[seq.length() + 1];
	char adapter_c[adapter.length() + 1];

	strcpy(seq_c, seq.c_str());
	strcpy(adapter_c, adapter.c_str());

	index = search(adapter_c, adapter.length(), seq_c, seq.length());


	for (auto &&i : index)
	{
        if (i >= 0)
		{
		    occurrences ++;
    		seq.erase(i, adapter.length());
	    	qual.erase(i, adapter.length());
        }
	}
}

void SingleFASTQ::trim(int qual_score, int minQuality, int minSequenceLength)
{

	convertQualToInteger(qual_score);
	int minSeq;

	for (int i = seq.length() ; i > 0; i--)
	{
		if (seq.at(i - 1) == 'N')
		{
			// cerr << "N Trim" << endl << endl;

			if (minSeq == minSequenceLength)
			{
				int j = i - 1;

				for (j ; j <= j - minSeq; j--)
				{
					seq.erase(j, 1);
					qual.erase(j, 1);
				}
				minSeq = 0;
			}
			else
			{
				// Not in minSequenceLength
				minSeq ++;
			}
			
		}
		if (minQuality != -1)
		{
			if (int_quality[i] < minQuality)
			{
				// cerr << "Quality Trim" << endl << endl;

				seq.erase(i, 1);
				qual.erase(i, 1);
			}
		}
	}
}

ostream &operator<<(ostream &os, const SingleFASTQ &single)
{
	os << single.id << endl;
	os << single.seq << endl;
	os << single.pholder << endl;
	os << single.qual << endl;
	return os;
}

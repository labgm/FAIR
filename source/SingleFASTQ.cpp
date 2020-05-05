#include "algos/core.cpp"

class SingleFASTQ
{
    int occurrences = 0;
	vector<int> int_quality;
	string id, seq, pholder, qual;
	friend ostream &operator<<(ostream &os, const SingleFASTQ &single);

public:
	bool SearchAdapter(string adapter, string seqi);
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
	void erase(string adapter, int mismatchMax);
	void trim(int qual_score, int minQuality, int minSequenceLength);
	void identify(string adapt);
	void setIdentifierAdapter(string idAdapter);
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

void SingleFASTQ::erase(string adapter, int mismatchMax)
{
	vector<int> index;

	char seq_c[seq.length() + 1];
	char adapter_c[adapter.length() + 1];

	strcpy(seq_c, seq.c_str());
	strcpy(adapter_c, adapter.c_str());

	if(mismatchMax > 0)
	index = searchMyers(adapter_c, adapter.length(), seq_c, seq.length(), mismatchMax);
	else
	index = searchShiftAnd(adapter_c, adapter.length(), seq_c, seq.length());

	// index = search(adapter_c, adapter.length(), seq_c, seq.length());

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

bool SingleFASTQ::SearchAdapter(string adapter, string seqi)
{
	vector<int> index;
	int mismatchMax = 0;

	char seq_c[seqi.length() + 1];
	char adapter_c[adapter.length() + 1];

	strcpy(seq_c, seqi.c_str());
	strcpy(adapter_c, adapter.c_str());

	index = searchMyers(adapter_c, adapter.length(), seq_c, seqi.length(), mismatchMax);

	for (auto &&i : index)
	{
        if (i >= 0)
		{
		    return true;
        }
	}

	return false;

}

void SingleFASTQ::trim(int qual_score, int minQuality, int minSequenceLength)
{
	convertQualToInteger(qual_score);
	int minSeq = 0;
	int posicaoInicial = -1;

	for (int i = seq.length() ; i > 0; i--)
	{
		if (seq.at(i - 1) == 'N')
		{

			minSeq ++;
			if (minSeq == (minSequenceLength))
			{
				posicaoInicial = i - 1 + (minSequenceLength);
			}
			
		}else{
			if(posicaoInicial != -1)
			{
				seq.erase(posicaoInicial-minSeq, minSeq);
				qual.erase(posicaoInicial-minSeq, minSeq);			
			}
				posicaoInicial = -1;
				minSeq = 0;
		}
		if (minQuality != -1)
		{
			if (int_quality[i] < minQuality)
			{
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

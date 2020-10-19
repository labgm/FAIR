#include "algos/core.cpp"
#include <algorithm>

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
	void erase(string adapter, int mismatchMax, string adapterInvert, double mismatchRight);
	void trim(int qual_score, int minQuality, int minSequenceLength);
	void identify(string adapt);
	void setIdentifierAdapter(string idAdapter);
	void removeAdapter(bool onlyRemove, string singleAdapter, int mismatchGlobal, string adapterInvert, double mismatchRight);
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
void SingleFASTQ::erase(string adapter, int mismatchMax, string adapterInvert, double mismatchRight)
{

	vector<int> index;
	vector<int> index_2;
	vector<int> index_3;

	char seq_c[seq.length() + 1];
	char adapter_c[adapter.length() + 1];

	strcpy(seq_c, seq.c_str());
	strcpy(adapter_c, adapter.c_str());

	if(mismatchMax > 0)
	index = searchMyers(adapter_c, adapter.length(), seq_c, seq.length(), mismatchMax, 0);
	else
	index = searchShiftAnd(adapter_c, adapter.length(), seq_c, seq.length());

	if(index.size() > 0)
	{

		for (int i = 0; i < index.size(); ++i)
		{

			// Process adapters for mismatch > 0. To find lower and upper index.
			if(index[i+1] != 0)
			{
				int limitSup = index[i] + adapter.length() - 1;
				int limitInf = index[i] - mismatchMax;
				if(limitInf < 0) limitInf = 0;

				string seq_aux_invert = "";
				for (int j = limitSup; j >= limitInf; --j)
				{
					seq_aux_invert += seq[j];
				}

				char seq_invert_c[seq_aux_invert.length() + 1];
				strcpy(seq_invert_c, seq_aux_invert.c_str());

				char adapter_invert_c[adapterInvert.length() + 1];
				strcpy(adapter_invert_c, adapterInvert.c_str());

				index_2 = searchMyers(adapter_invert_c, adapterInvert.length(), seq_invert_c, seq_aux_invert.length(), mismatchMax, 0);

				if(index_2.size() > 0)
				{
					for (int j = 0; j < index_2.size(); ++j)
					{

						if(seq.length() > 0)
						{

							int limitInf = (index[i] + adapter.length()) - (index_2[j] + adapter.length());
							if (limitInf < 0) limitInf = 0;

							int sizeCorte = index[i] + adapter.length() - limitInf;
							int limitSup = limitInf + sizeCorte;	

							if(sizeCorte > 0 & limitSup <= seq.length())
							{

								if(limitInf >= (seq.length() - adapter.length()))
								{
									sizeCorte = seq.length() - limitInf;
								}

					    		seq.erase(limitInf, sizeCorte);
						    	qual.erase(limitInf, sizeCorte);

								occurrences ++;
					    	}

							++j;

						}
					}
				}

			// Process adapters without mismatch
			}else if(index[i+1] == 0){

				int sizeCorte = adapter.length();
				int limitSup = index[i] + sizeCorte;

				if(sizeCorte > 0 & limitSup <= seq.length())
				{

					// If trim out of the sequence, reduce the trim to the end of read.
					if(index[i] >= (seq.length() - adapter.length()))
					{
						sizeCorte = seq.length() - index[i];
					}

					seq.erase(index[i], sizeCorte);
					qual.erase(index[i], sizeCorte);

						occurrences ++;
				}

			}

			++i;

		}

	}else{

		// If the adapter is not found, search only at the end of the read (3')

		int taxaMismatchAdapter_extrem_int = mismatchRight * adapter.length();

		if(taxaMismatchAdapter_extrem_int > 0)
		{

			int indiceStart = seq.length() - taxaMismatchAdapter_extrem_int - 1;

			// Search in 3'
			index_3 = searchMyers(adapter_c, adapter.length(), seq_c, seq.length(), taxaMismatchAdapter_extrem_int, indiceStart);

			if(index_3.size() > 0)
			{

				int limitSup = index_3[0] + adapter.length() - 1;
				int limitInf = index_3[0] - mismatchMax;
				if(limitInf < 0) limitInf = 0;

				string seq_aux_invert = "";
				for (int j = limitSup; j >= limitInf; --j)
				{
					seq_aux_invert += seq[j];
				}

				char seq_invert_c[seq_aux_invert.length() + 1];
				strcpy(seq_invert_c, seq_aux_invert.c_str());

				char adapter_invert_c[adapterInvert.length() + 1];
				strcpy(adapter_invert_c, adapterInvert.c_str());

				// Search reverse in 3'
				index_2 = searchMyers(adapter_invert_c, adapterInvert.length(), seq_invert_c, seq_aux_invert.length(), taxaMismatchAdapter_extrem_int, 0);

				if(index_2.size() > 0)
				{
						
					int limitInf = (index_3[0] + adapter.length()) - (index_2[0] + adapter.length());
					int sizeCorte = index_3[0] + adapter.length() - limitInf;
					int limitSup = limitInf + sizeCorte;	

					if(sizeCorte > 0 & limitSup <= seq.length())
					{

			    		seq.erase(limitInf, sizeCorte);
				    	qual.erase(limitInf, sizeCorte);

						occurrences ++;
			    	}

				}

			}

		}
		
	}

}

bool SingleFASTQ::SearchAdapter(string adapter, string seqi)
{
	vector<int> index;
	int mismatchMax = 3;

	char seq_c[seqi.length() + 1];
	char adapter_c[adapter.length() + 1];

	strcpy(seq_c, seqi.c_str());
	strcpy(adapter_c, adapter.c_str());

	index = searchMyers(adapter_c, adapter.length(), seq_c, seqi.length(), mismatchMax, 0);

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

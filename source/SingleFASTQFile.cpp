#include "PairedFASTQ.cpp"
#include <algorithm>

class SingleFASTQFile
{
	SingleFASTQ sequence, identAdapter;
	string file, adapter;
	ofstream fout;
	ifstream fin;
	int quality; // 33 or 64

	vector<string> adaptersVec;
	vector<int> adaptersVecQuant;

public:
	bool openFASTQInput(string file, int quality);
	bool openFASTQOutput(string file);
	bool hasNext();
	SingleFASTQ getNext();
	string identifyAdapter();
	void identifyQuality();
	void trim(int minQuality, int minSequenceLength);
	void removeAdapter(bool onlyRemove, string adapter, int mismatchMax);
	void write();
	void closeOutput();

	bool SearchAdapters(string seq, string typeRead);
	bool hasNextSearchAdapters(string typeRead);

	vector<string> getAdaptersVec();
	vector<int> getAdaptersVecQuant();

	void writeOnlyIdentifyHeader(string type);
	void writeOnlyIdentify(string text);

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

bool SingleFASTQFile::hasNextSearchAdapters(string typeRead)
{

	string lines[4];

	for (int i = 0; i < 4; i++)
		if (!getline(fin, lines[i]))
			return false;

	string seq = lines[1];

	if(SearchAdapters(seq, typeRead))
	{
		return true;
	}

	return false;

}

bool SingleFASTQFile::SearchAdapters(string seq, string typeRead)
{

  string adapt;
  ifstream myfile("adapters.txt");
  int i = 1;
  if (myfile.is_open())
  {
    while (!myfile.eof() )
    {
      getline (myfile,adapt);
      if(adapt != "" & (i%2) == 0)
      {
	  		if(identAdapter.SearchAdapter(adapt, seq))
	  		{
	  			if(typeRead == "forward")
	  			{
	  				string aux = adapt;
	  				// string aux = "Forward: "+adapt;
	  				// cerr << aux << endl;
	  				if (std::find(this->adaptersVec.begin(), this->adaptersVec.end(), aux) != this->adaptersVec.end())
					{
	  					for (int i = 0; i < this->adaptersVec.size(); ++i)
	  					{
	  						if(this->adaptersVec[i] == aux)
	  						{
	  							this->adaptersVecQuant[i] += 1;
	  							break;
	  						}
	  					}
	  				}else{
	  					this->adaptersVec.push_back(aux);
	  					this->adaptersVecQuant.push_back(1);
	  				}
	  			}
	  			else if(typeRead == "reverse")
	  			{
	  				string aux = adapt;
	  				// string aux = "Reverse: "+adapt;
	  				// cerr << aux << endl;
	  				if (std::find(this->adaptersVec.begin(), this->adaptersVec.end(), aux) != this->adaptersVec.end())
					{
	  					for (int i = 0; i < this->adaptersVec.size(); ++i)
	  					{
	  						if(this->adaptersVec[i] == aux)
	  						{
	  							this->adaptersVecQuant[i] += 1;
	  							break;
	  						}
	  					}
	  				}else{
	  					this->adaptersVec.push_back(aux);
	  					this->adaptersVecQuant.push_back(1);
	  				}
	  			}
	  			// STOP SEARCH (PARA CONSIDERAR PRIMEIRO ADAPTADOR ENCONTRADO)
	  			// return false;
	  		}

      }
      	 i += 1;
    }
    myfile.close();
  }

  return true;

}

vector<string> SingleFASTQFile::getAdaptersVec()
{
	return adaptersVec;
}
vector<int> SingleFASTQFile::getAdaptersVecQuant()
{
	return adaptersVecQuant;
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

void SingleFASTQFile::trim(int minQuality, int minSequenceLength)
{
	sequence.trim(quality, minQuality, minSequenceLength);
}

void SingleFASTQFile::removeAdapter(bool onlyRemove, string adapter, int mismatchMax)
{

	if (onlyRemove) // Adapter as Parameter
	{
		this->adapter = adapter;
	}
	else // Need to Identify
	{
		adapter = identifyAdapter();
	}

	sequence.erase(adapter, mismatchMax);

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

void SingleFASTQFile::writeOnlyIdentifyHeader(string type)
{
	if(type == "forward")
	fout << "- Found Adapters - Forward" << "\n\n";
	if(type == "reverse")
	fout << "- Found Adapters - Reverse" << "\n\n";

	fout << "Adapter\tAmount" << "\n";
}

void SingleFASTQFile::writeOnlyIdentify(string text)
{
	fout << text << "\n";
}

void SingleFASTQFile::closeOutput()
{
	cerr << "Number of Occurrences: " << sequence.getOccurrences() << endl;
	fin.close();
	fout.close();
}
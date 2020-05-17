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
	void erase(string adapter, int mismatchMax, string adapterInvert);
	void trim(int qual_score, int minQuality, int minSequenceLength);
	void identify(string adapt);
	void setIdentifierAdapter(string idAdapter);

	void insert(string adapter, bool toInsert, double adapterErrorRate, bool adapterInsertionLeft, bool adapterRandomPosition);

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

void SingleFASTQ::erase(string adapter, int mismatchMax, string adapterInvert)
{

	vector<int> index;
	vector<int> index_2;
		    // cerr << seq << endl;

	char seq_c[seq.length() + 1];
	char adapter_c[adapter.length() + 1];

	strcpy(seq_c, seq.c_str());
	strcpy(adapter_c, adapter.c_str());

	if(mismatchMax > 0)
	index = searchMyers(adapter_c, adapter.length(), seq_c, seq.length(), mismatchMax);
	else
	index = searchShiftAnd(adapter_c, adapter.length(), seq_c, seq.length());

	//PEGAR PARTE DE SEQUENCIA E INVERTER
	if(index.size() > 0)
	{

		// if(1 == 1)
		// {

		// 	for (int i = 0; i < index.size(); ++i)
		// 	{
		// 		int sizeCorte = adapter.length();
		// 		int limit = index[i] + sizeCorte;

		// 		if(index[i] < 0) index[i] = 0;
		// 		while((index[i] + sizeCorte) >= seq.length()) sizeCorte -= 1;

		// 		seq.erase(index[i], sizeCorte);
		// 		qual.erase(index[i], sizeCorte);

		// 				occurrences ++;
		// 		++i;
		// 	}

		// }else{

		// PRECISION MODE
		for (int i = 0; i < index.size(); ++i)
		{

			//SE MISMATCH MIN ENCONTRADO FOR DIFERENTE DE 0 -> CRIAR REVERSE DE PARTE DA SEQUECIA
			if(index[i+1] != 0)
			{
				int limitSup = index[i] + adapter.length() - 1;
				int limitInf = index[i] - mismatchMax;

				string seq_aux_invert = "";
				for (int j = limitSup; j >= limitInf; --j)
				{
					seq_aux_invert += seq[j];
				}

				char seq_invert_c[seq_aux_invert.length() + 1];
				strcpy(seq_invert_c, seq_aux_invert.c_str());

				char adapter_invert_c[adapterInvert.length() + 1];
				strcpy(adapter_invert_c, adapterInvert.c_str());

				index_2 = searchMyers(adapter_invert_c, adapterInvert.length(), seq_invert_c, seq_aux_invert.length(), mismatchMax);

				if(index_2.size() > 0)
				{
					for (int j = 0; j < index_2.size(); ++j)
					{

						int limitInf = index[i] + adapter.length() - (index_2[j] + adapter.length());
						if (limitInf < 0) limitInf = 0;

						int quantCorte = index[i] + adapter.length() - limitInf;
						while((limitInf + quantCorte) >= seq.length()) --quantCorte;

						occurrences ++;

						// cerr << "limitInf: " << limitInf << endl;
						// cerr << "quantCorte: " << quantCorte << endl;
			    		
			    		seq.erase(limitInf, quantCorte);
				    	qual.erase(limitInf, quantCorte);

				    	// erro coredump

						++j;
					}
				}

			}else if(index[i+1] == 0){

				int sizeCorte = adapter.length();
				int limit = index[i] + sizeCorte;

				if(index[i] < 0) index[i] = 0;
				while((index[i] + sizeCorte) >= seq.length()) sizeCorte -= 1;

				seq.erase(index[i], sizeCorte);
				qual.erase(index[i], sizeCorte);

						occurrences ++;

			}

			++i;

		}
		// }
	}
			    // cerr << seq << endl;
			    // cerr << qual << endl;
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

void SingleFASTQ::insert(string adapter, bool toInsert, double adapterErrorRate, bool adapterInsertionLeft, bool adapterRandomPosition)
{

	if(toInsert)
	{

	// bool adapterInsertionLeft = false; // Inserir adaptadores na esquerda ou direta
	// bool adapterRandomPosition = true; // se será inserido em posicoes aleatorias (seguindo 'adapterInsertionLeft' LEFT or RIGHT da read)
	// double adapterErrorRate = 0.7; // taxa de erro máxima nos adaptadores (Ex: 70% do adaptador pode sofrer alteração)
	int ratio = 20; // Distância máxima de bases do adaptador das extremidades da read(LEFT or RIGHT)
	
	int maxErrors = adapterErrorRate * (adapter.length());
		// cerr << "maxErrors: " << maxErrors << endl;

	int quantErrors = rand() % (maxErrors+1) + 0; //Gerando erros de 0 até maxErrors
		// cerr << "quant errors1: " << quantErrors << endl;

	// CRIANDO NOVO ADAPTADOR COM ERROS - PT1
	vector<int> vecErrorPositions;

	for (int i = 0; i < quantErrors; ++i)
	{
		// int posi = rand() % (adapter.length()) + 0; //PEGANDO POSICOES ALEATORIAS DO ADAPTADOR
		int posi = rand() % (adapter.length() - 5) + 5; //PEGANDO POSICOES ALEATORIAS DO ADAPTADOR P/ teste com marcadores especiais -> XXXXXADAPTERAQUIZZZZZ
		if (std::find(vecErrorPositions.begin(), vecErrorPositions.end(), posi) != vecErrorPositions.end())
		{
			// contém
			i--;
		}else{
			// não contém
			vecErrorPositions.push_back(posi);
			// cerr << "Posi: " << posi << endl;			
		}
	}

	// PT2
	if(vecErrorPositions.size() > 0)
	{

	string adapter_aux = "";

		for (int i = 0; i < adapter.length(); ++i)
		{
			// SE POSICAO DO VETOR ESTÁ DENTRO DO VETOR DE POSICOES A SER MODIFICADO
			if (std::find(vecErrorPositions.begin(), vecErrorPositions.end(), i) != vecErrorPositions.end())
			{
				int typeError = rand() % 3 + 1;
				if(typeError == 1) 
				{
					// Erro de Substituição
					string bases = "ACTG";
					int randomBase = rand() % 4 + 0;
					adapter_aux += bases[randomBase];
				}
				else if(typeError == 2)
				{
					//ERRO de inserção
					string bases = "ACTG";
					int randomBase = rand() % 4 + 0;
					adapter_aux += adapter[i] + bases[randomBase];
				}

			}else{
				adapter_aux += adapter[i];
			}
		}
		
		adapter = adapter_aux;
	}

	string temp_qual = ",";

	// GERANDO VALOR STRING DE QUALIDADE
	for (int i = 0; i < (adapter.length()-1); ++i) temp_qual += ",";


	// GERANDO STRINGS DE SEQUENCIA E QUALIDADE
	string aux_seq = "";
	string aux_qual = "";
	if(ratio < seq.length())
	{

	if(adapterRandomPosition)
	{

		int positionRand = 0;
 		positionRand = rand() % ratio + 1; // +1 é obrigatorio

		if(adapterInsertionLeft == true)
		{
			for (int i = 0; i < seq.length(); ++i)
			{
				if(i == positionRand)
				{
					aux_seq += adapter;
					aux_qual += temp_qual;
				
				}else{

					aux_seq += seq[i];
					aux_qual += qual[i];
				}
			}

		}else{

			int posiRight = seq.length() - positionRand;

			for (int i = 0; i < seq.length(); ++i)
			{
				if(i == posiRight)
				{
					aux_seq += adapter;
					aux_qual += temp_qual;
				
				}else{

					aux_seq += seq[i];
					aux_qual += qual[i];

				}
			}
		}

		seq = aux_seq;
		qual = aux_qual;

	}else{
		
		if(adapterInsertionLeft)
		{
			seq = adapter+seq;
			qual = temp_qual+qual;

		}else{

			seq += adapter;
			qual += temp_qual;
		}
	}

	}

	}

	// cerr << "seq: " << seq << endl;
	// cerr << "qua: " << qual << endl;


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

#include "algos/core.cpp"
#include <algorithm>

class SingleFASTQ
{
	int occurrences = 0;
	vector<int> int_quality;
	string id, seq, pholder, qual;
	friend ostream &operator<<(ostream &os, const SingleFASTQ &single);

	// Quality Parameters
	bool readAffected = false;
	// count trimming bases affecteds Quality Trimming, Flanks, Ns . 
	int count_affected_bases = 0;

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
	void erase(string adapter, int mismatchMax, string adapterInvert, double mismatchRight, double five_dr, double rre);
	void trim(int qual_score, int minQuality, int sizeQualityWindow, bool trimNFlank, int maxN);
	void identify(string adapt);
	void setIdentifierAdapter(string idAdapter);
	bool returnReadAffected();

	int returnNAffectedBases();
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

void SingleFASTQ::erase(string adapter, int mismatchMax, string adapterInvert, double mismatchRight, double five_dr, double rre)
{

	vector<int> index;
	vector<int> index_2;
	vector<int> index_3;

	char seq_c[seq.length() + 1];
	char adapter_c[adapter.length() + 1];

	strcpy(seq_c, seq.c_str());
	strcpy(adapter_c, adapter.c_str());

	int posi_inicial_baseado_in_ration = five_dr * (seq.length());

	index = searchMyers(adapter_c, adapter.length(), seq_c, seq.length(), mismatchMax, posi_inicial_baseado_in_ration);

	if (index.size() > 0)
	{

		// cerr << "Maior que 0" << endl;

		for (int i = 0; i < index.size(); ++i)
		{

			// Process adapters for mismatch > 0. To find lower and upper index.
			if (index[i + 1] != 0)
			{
				int limitSup = index[i] + adapter.length() - 1;
				int limitInf = index[i] - mismatchMax;
				if (limitInf < 0)
					limitInf = 0;

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

				if (index_2.size() > 0)
				{
					for (int j = 0; j < index_2.size(); ++j)
					{

						if (seq.length() > 0)
						{

							int limitInf = (index[i] + adapter.length()) - (index_2[j] + adapter.length());
							if (limitInf < 0)
								limitInf = 0;

							int sizeCorte, limitSup;

							if(rre == false){
								sizeCorte = index[i] + adapter.length() - limitInf - 1;
								limitSup = limitInf + sizeCorte;
							}else{
								sizeCorte = seq.length() - limitInf + 1;
								limitSup = seq.length() - 1;
							}

							if (sizeCorte > 0 & limitSup <= seq.length())
							{

								if (limitInf >= (seq.length() - adapter.length()))
								{
									sizeCorte = seq.length() - limitInf;
								}

								seq.erase(limitInf, sizeCorte);
								qual.erase(limitInf, sizeCorte);

								occurrences++;
							}

							++j;
						}
					}
				}

				// Process adapters without mismatch
			}
			else if (index[i + 1] == 0)
			{

				int limitInf;
				int sizeCorte;
				int limitSup;
				
				if(rre == false){
				// Corte somente do adaptador
					sizeCorte = adapter.length();
					limitSup = index[i] + sizeCorte;
				}else{
					// Corte toda extremidade restante
					limitInf = index[i];
					sizeCorte = seq.length() - (limitInf+1) + 1;
					limitSup = limitInf + sizeCorte;
				}

				if (sizeCorte > 0 & limitSup <= seq.length())
				{

					// If trim out of the sequence, reduce the trim to the end of read.
					if (index[i] >= (seq.length() - adapter.length()))
					{
						sizeCorte = seq.length() - index[i];
					}

					seq.erase(index[i], sizeCorte);
					qual.erase(index[i], sizeCorte);

					occurrences++;
				}
			}

			++i;
		}
	}
	else
	{

		// If the adapter is not found, search only at the end of the read (3')

		int taxaMismatchAdapter_extrem_int = mismatchRight * adapter.length();

		if (taxaMismatchAdapter_extrem_int > 0)
		{

			int indiceStart = seq.length() - taxaMismatchAdapter_extrem_int - 1;

			// Search in 3'
			index_3 = searchMyers(adapter_c, adapter.length(), seq_c, seq.length(), taxaMismatchAdapter_extrem_int, indiceStart);

			if (index_3.size() > 0)
			{

				int limitSup = index_3[0] + adapter.length() - 1;
				int limitInf = index_3[0] - mismatchMax;
				if (limitInf < 0)
					limitInf = 0;

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

				if (index_2.size() > 0)
				{

					int limitInf = (index_3[0] + adapter.length()) - (index_2[0] + adapter.length());
					int sizeCorte = index_3[0] + adapter.length() - limitInf;
					int limitSup = limitInf + sizeCorte;

					if (sizeCorte > 0 & limitSup <= seq.length())
					{

						seq.erase(limitInf, sizeCorte);
						qual.erase(limitInf, sizeCorte);

						occurrences++;
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

void SingleFASTQ::trim(int qual_score, int minQuality, int sizeQualityWindow, bool trimNFlank, int maxN)
{

	bool trim_n_flank = trimNFlank;
	int max_n = maxN;

	readAffected = false;
	count_affected_bases = 0;

	int bkp_len_seq = seq.length();

	convertQualToInteger(qual_score);
	int minSeq = 0;
	int posicaoInicial = -1;
	
	/*
	- Quality removal -
	Method similar to AdapterRemoval (https://adapterremoval.readthedocs.io/en/latest/manpage.html)
	The idea is to determine new start (5') and end (3') positions for the read. 
	For this, in forward direction 5' -> 3' marks as the new position 5' the first window with an 
	average greater than minQuality the first base of the kmer must also be greater than minQuality. 
	From the previous position, the window slides towards 3' and marks as new position 3' first window with 
	minQuality average less than or equal. The new 3 'is placed at the bottom of that window where the quality 
	is greater than or equal to minQuality. If no 5 'position can be determined, the reading is discarded.
	*/

	// +++  minQuality  +++ Remove sequências por qualidade +++++++
	if (minQuality != -1 && (seq.length() > 0))
	{
		// Se o tamanho da leitura for menor que o tamanho da janela de qualidade, utilizar o tamanho total da leitura como janela
		if (seq.length() < sizeQualityWindow)
			sizeQualityWindow = seq.length();

		int novo_5 = -1; // -1, valor de inicialização. Significando que se manter '-1' não foi encontrado nova posição 5'
		int novo_3 = -1;

		for (int i = 0; i < seq.length(); i++)
		{
			// cerr << "Qual (" << i << "): " << int_quality[i] << endl;

			// Iteração para definição de média de qualidade do tamanho da janela (sizeQualityWindow)
			double windowAverage = 0.0;
			int indic = 0;
			int cont_ind = 0;
			int k = i;
			int limit_sup = k + sizeQualityWindow;
			for (k = i; k < limit_sup; k++)
			{
				if (indic < seq.length()) // preserva limite para não ultrapassar a quantidade de bases
				{
					cont_ind++;
					indic = k;
					windowAverage += int_quality[indic];
				}
				else
					break;
			}

			// Tira a média de qualidade da janela
			windowAverage = windowAverage / cont_ind;

			// Regra I
			// Identificar primeira janela com qualidade superior à minQuality
			if (windowAverage > minQuality && int_quality[i] > minQuality)
			{
				// Se novo_5 ainda não foi selecionado, marcar posicao e avancar 'i'
				if (novo_5 == -1)
				{
					novo_5 = i;
					i += cont_ind;
				}
			}
			else if (windowAverage <= minQuality)
			{
				// Regra que permite só marcar novo_3' se novo_5' já tiver sido marcado
				if (novo_5 != -1)
				{
					for (int w = 0; w < sizeQualityWindow; w++)
					{
						if (int_quality[i + w] >= minQuality)
						{
							novo_3 = i + w;
							break;
						}
					}
				}
			}

			// Finaliza iterações quando janela chegar na extremidade 3'
			if (i >= (seq.length() - sizeQualityWindow))
				break;
		}

		// Verificar novos 5' e 3'

		// Verifica se foi encontrado novo 5' mas não um novo 3' remove fragmento do inicio até nova posicao 5'
		if (novo_3 == -1 && novo_5 != -1)
		{
			seq.erase(0, (novo_5));
			qual.erase(0, (novo_5));

			// Marca que a read foi afetada pelo corte de qualidade	e quantas bases foram cortadas
			readAffected = true;
			count_affected_bases += novo_5;
		}
		// Se foram definidos novos 3' e 5', remover os fragmentos
		else if (novo_3 != -1)
		{
			// Cortando extremidade 3'
			int quant_corte_3 = (seq.length() - novo_3);
			seq.erase(novo_3, quant_corte_3);
			qual.erase(novo_3, quant_corte_3);

			// Marca que a read foi afetada pelo corte de qualidade	e quantas bases foram cortadas
			readAffected = true;
			count_affected_bases += quant_corte_3;

			// Cortando extremidade 5'
			seq.erase(0, (novo_5));
			qual.erase(0, (novo_5));

			count_affected_bases += novo_5;
		}
		// Caso não foi encontrado região com qualidade maior que minQuality, apagar sequência
		else if (novo_5 == -1)
		{
			readAffected = true;
			count_affected_bases += seq.length();

			seq = "";
			qual = "";
		}
	}

	// +++  trim_n_flank  +++ Removendo Ns de extremidades da sequência ++++++
	// Verifica se usuario ativou parâmetro (trim_n_flank) e, se sequencia não está vazia após trimming por qualidade, a fim de economizar esforços
	if (trim_n_flank && (seq.length() > 0))
	{
		// Removendo Ns do inicio da sequencia
		while (seq[0] == 'N' || seq[0] == 'n')
		{
			seq.erase(0, 1);
			qual.erase(0, 1);
			count_affected_bases += 1;
		}
		// Removendo Ns do final da sequencia
		int ind_end = seq.length() - 1;
		while (seq[ind_end] == 'N' || seq[ind_end] == 'n')
		{
			seq.erase(ind_end, 1);
			qual.erase(ind_end, 1);
			ind_end = seq.length() - 1;
			count_affected_bases += 1;
		}
	}

	// +++  max-n  +++ Removendo leitura baseado na quantidade de Ns ++++++
	// Verifica se usuario ativou parametro(max_n != -1) e, se leitura > 0 a fim de economizar esforços
	if (max_n != -1 && (seq.length() > 0))
	{
		int count_n = 0;
		for (int i = 0; i < seq.length(); ++i)
		{
			if (seq[i] == 'N' || seq[i] == 'n')
				++count_n;
		}
		// Remove toda leitura, caso exceda a quantidade maxima de Ns permitido
		if (count_n > max_n)
		{
			count_affected_bases += seq.length();
			seq = "";
			qual = "";
		}
	}

	// Verifica se não houve corte
	if (bkp_len_seq == (seq.length()))
		readAffected = false;

	// cerr << "Count_Bases_Cut: " << count_affected_bases << endl;
}

ostream &operator<<(ostream &os, const SingleFASTQ &single)
{
	os << single.id << endl;
	os << single.seq << endl;
	os << single.pholder << endl;
	os << single.qual << endl;
	return os;
}

bool SingleFASTQ::returnReadAffected()
{

	return readAffected;
}

int SingleFASTQ::returnNAffectedBases()
{

	return count_affected_bases;
}
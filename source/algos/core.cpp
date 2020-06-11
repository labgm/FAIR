// MYERS 1999
//BUSCA O PADRÃO EM TODA A SEQUENCIA

#include "include/define.h"
#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <dirent.h>
#include <errno.h>
#include <ctime>
#define GRAM4(j) (B[y[j]] << 3) & (B[y[j - 1]] << 2) & (B[y[j - 2]] << 1) & B[y[j - 3]]
#include <string.h>

using namespace std;

vector<int> searchMyers(char *P, long m, char *T, long n, int mismatchMax){

	int k = mismatchMax;
	
	vector<int> indexx;

	int MAXCHAR = 256;
	int posicaoAnterior, quantGrupo, posicaoAtual, indiceMedio;

	// ================== VARIAVEIS RESPONSAVEIS NO PROCESSO DE DEFINIR INDICES CORRETOS =====================
	int inicializador = 0;
	posicaoAnterior = 0;
	quantGrupo = 0;
	posicaoAtual = 0;
	indiceMedio = 0;
	// =======================================================================================================

	// ComputePM ////////////////////////////////////////
	long PM[MAXCHAR];
	for (int ii = 0; ii < MAXCHAR; ii++) PM[ii] = 0;
	for (int ii = 1; ii <= m; ii++){
		long temp = 1 << (ii-1);
		PM[P[ii-1]+127] = PM[P[ii-1]+127] | temp;
		// cerr << ii - 1 << ": " << PM[P[ii-1]+127] << endl;
	}
	////////////////////////////////////////////////////

	long VN = 0;
	long VP = 0xffffffff;
	long D0, HP, HN;

	long currDist = m;
	long currDistAnt = 0;
	int minorCurr = k;
	int posiMinorCurr;
	bool ultimo = false;

	for (int j = 0; j < n; ++j){

		posicaoAtual = j+1;

		// Step(j); ///////////////////////////////////////////////
		D0 = (((PM[T[j]+127] & VP) + VP) ^ VP) | PM[T[j]+127] | VN;
		HP = VN | ~ (D0 | VP);
		HN = VP & D0;
		VP = (HN << 1) | ~ (D0 | (HP << 1));
		VN = (HP << 1) & D0;
		///////////////////////////////////////////////////////////

		if ((HP & 0x80000000) == 0x80000000) currDist += 1;
		else if((HN & 0x80000000) == 0x80000000) currDist -= 1;

		// SE ESTÁ SE APROXIMANDO DA EXTREMIDADE AUMENTAR QUANTIDADE DE MISMATCH
		if (j >= (n - (m/2)))
		{
			k = m / 3;
			// ultimo = true;
		}

		if (currDist <= k){

			inicializador++;

				if(inicializador == 1)
				{

					quantGrupo++;
					minorCurr = currDist;
					posiMinorCurr = posicaoAtual;

				}else{

					if(currDist < minorCurr)
					{
						minorCurr = currDist;
						posiMinorCurr = posicaoAtual;
					}
				}
				
		}else{

			if(quantGrupo >= 1)
			{
				if(ultimo == true) minorCurr = minorCurr * (-1);

				indexx.insert(indexx.begin(), minorCurr);
				indexx.insert(indexx.begin(), posiMinorCurr - m);

				inicializador = 0;
				quantGrupo = 0;
				minorCurr = k;

			}

		}

		if (posicaoAtual == n){
			if(quantGrupo >= 1){

				if(ultimo == true) minorCurr = minorCurr * (-1);

					indexx.insert(indexx.begin(), minorCurr);
					indexx.insert(indexx.begin(), posiMinorCurr - m);
			}
	    }
	}

	return indexx;

	}


vector<int> searchMyers_2(char *P, long m, char *T, long n, int mismatchMax){

	int k = mismatchMax;
	
	vector<int> indexx;

	int MAXCHAR = 256;
	int posicaoAnterior, quantGrupo, posicaoAtual, indiceMedio;

	// ================== VARIAVEIS RESPONSAVEIS NO PROCESSO DE DEFINIR INDICES CORRETOS =====================
	int inicializador = 0;
	posicaoAnterior = 0;
	quantGrupo = 0;
	posicaoAtual = 0;
	indiceMedio = 0;
	// =======================================================================================================

	// ComputePM ////////////////////////////////////////
	long PM[MAXCHAR];
	for (int ii = 0; ii < MAXCHAR; ii++) PM[ii] = 0;
	for (int ii = 1; ii <= m; ii++){
		long temp = 1 << (ii-1);
		PM[P[ii-1]+127] = PM[P[ii-1]+127] | temp;
		// cerr << ii - 1 << ": " << PM[P[ii-1]+127] << endl;
	}
	////////////////////////////////////////////////////

	long VN = 0;
	long VP = 0xffffffff;
	long D0, HP, HN;

	long currDist = m;
	long currDistAnt = 0;
	int minorCurr = k;
	int posiMinorCurr;
	bool ultimo = false;

	for (int j = 0; j < n; ++j){

		posicaoAtual = j+1;

		// Step(j); ///////////////////////////////////////////////
		D0 = (((PM[T[j]+127] & VP) + VP) ^ VP) | PM[T[j]+127] | VN;
		HP = VN | ~ (D0 | VP);
		HN = VP & D0;
		VP = (HN << 1) | ~ (D0 | (HP << 1));
		VN = (HP << 1) & D0;
		///////////////////////////////////////////////////////////

		if ((HP & 0x80000000) == 0x80000000) currDist += 1;
		else if((HN & 0x80000000) == 0x80000000) currDist -= 1;

		if (currDist <= k){

			inicializador++;

				if(inicializador == 1)
				{

					quantGrupo++;
					minorCurr = currDist;
					posiMinorCurr = posicaoAtual;

				}else{

					if(currDist < minorCurr)
					{
						minorCurr = currDist;
						posiMinorCurr = posicaoAtual;
					}
				}
				
		}else{

			if(quantGrupo >= 1)
			{
				indexx.insert(indexx.begin(), minorCurr);
				indexx.insert(indexx.begin(), posiMinorCurr - m);

				inicializador = 0;
				quantGrupo = 0;
				minorCurr = k;

			}

		}

		if (posicaoAtual == n){
			if(quantGrupo >= 1){

					indexx.insert(indexx.begin(), minorCurr);
					indexx.insert(indexx.begin(), posiMinorCurr - m);
			}
	    }
	}

	return indexx;

	}

vector<int> searchShiftAnd(char *P, long m, char *T, long n){

//	K = NUMERO MAXIMO DE ERROS PERMITIDOS (DELECAO, SUBSTITUICAO, INSERCAO)
	int k = 0;

	// ARMAZENA INDICES DE POSICOES CORRETAS DE MATCH APROXIMADO
	vector<int> indexx;

	// ===== VARIAVEIS RESPONSAVEIS NO PROCESSO DE DEFINIR INDICES CORRETOS DE MATCH NO CASAMENTO APROXIMADO
			int inicializador = 0;
			int posicaoAnterior = 0;
			int quantGrupo = 0;
			int posicaoAtual = 0;
			int indiceMedio = 0;
	// =======================================================================================================

	int MAXCHAR = 256;

	long Masc[MAXCHAR], i, j, Ri, Rant, Rnovo;
		// R[2]
	long R[k];

	for (i = 0; i < MAXCHAR; i++) Masc[i] = 0;

	for (i = 1; i <= m; i++){
			Masc[P[i-1]+127] |= 1 << (m-i);
		 } 

		R[0] = 0;
		Ri = 1 << (m-1);
		for (j = 1; j <= k; j++) R[j] = (1 << (m-j)) | R[j-1];

		for (i = 0; i < n; i++){

			Rant = R[0];
			Rnovo = ((((unsigned long)Rant) >> 1) | Ri) & Masc[T[i] + 127];
			R[0] = Rnovo;

			for (j = 1; j <= k; j++)
			{

				Rnovo = ((((unsigned long) R[j]) >> 1) & Masc[T[i] + 127]) | Rant | (((unsigned long)(Rant | Rnovo))  >> 1);
				Rant = R[j];
				R[j] = Rnovo | Ri;

			}

			posicaoAtual = i+1;

				// cerr << posicaoAtual << endl;
			// SE VERDADEIRO - ACEITOU MATCH COM QUANTIDADE DE ERROS PERMITIDO
			if((Rnovo & 1) != 0){

				indexx.insert(indexx.begin(), k);
				indexx.insert(indexx.begin(), (posicaoAtual - m));

			}

		}

	 	// cerr << "--------" << endl;

	 return indexx;


}
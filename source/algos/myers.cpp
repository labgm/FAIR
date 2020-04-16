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
	int posicaoAnterior, quantGrupo, posicaoAtual, indiceMedio, acumulaPmedia;

	// ================== VARIAVEIS RESPONSAVEIS NO PROCESSO DE DEFINIR INDICES CORRETOS =====================
	int inicializador = 0;
	posicaoAnterior = 0;
	quantGrupo = 0;
	posicaoAtual = 0;
	indiceMedio = 0;
	acumulaPmedia = 0;
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

		// MATCH =========
		if (currDist <= k){

			currDistAnt = currDist;

			inicializador++;

				if(inicializador == 1){

					quantGrupo++;
					posicaoAnterior = posicaoAtual;
					acumulaPmedia += posicaoAtual;

				}
				else{

					// se posicao anterior +1 eh igual a posicao atual (significa que eh parte do grupo)
					if((posicaoAnterior+1) == posicaoAtual){

						quantGrupo++;
						posicaoAnterior = posicaoAtual;
						acumulaPmedia += posicaoAtual;

					}
					else{

						indiceMedio = acumulaPmedia/quantGrupo;
						indexx.insert(indexx.begin(), (indiceMedio - m) + currDistAnt);

						acumulaPmedia = posicaoAtual;
						posicaoAnterior = posicaoAtual;
						quantGrupo = 1;
						inicializador = 1;
						
				}

		    }

		}

		if (posicaoAtual == n){
			if(quantGrupo >= 1){
					indiceMedio = acumulaPmedia/quantGrupo;
					indexx.insert(indexx.begin(), (indiceMedio - m) + currDistAnt);
			}
	    }
	}


	return indexx;

	}


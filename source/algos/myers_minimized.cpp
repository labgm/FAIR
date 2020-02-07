//VERSÃO QUE PÁRA LEITURA DA SEQUÊNCIA AO ENCONTRAR O PRIMEIRO PADRÃO

//ESTA VERSÃO ESTÁ DEIXANDO PARA TRÁS ALGUNS ADAPTADORES DE CASAMENTO EXATO

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

vector<int> search(char *P, long m, char *T, long n){

	long k = 3;
	
	vector<int> indexx;

	int MAXCHAR = 256;
	int posicaoAnterior, quantGrupo, posicaoAtual, indiceMedio, acumulaPmedia;
	bool match = false;

	// ================== VARIAVEIS RESPONSAVEIS NO PROCESSO DE DEFINIR INDICES CORRETOS =====================
	quantGrupo = 0;
	posicaoAtual = 0;
	indiceMedio = 0;
	acumulaPmedia = 0;
	// =======================================================================================================

	// ComputePM ////////////////////////////////////////
	long PM[MAXCHAR];
	for (int x = 0; x < MAXCHAR; x++) PM[x] = 0;
	for (int x = 1; x <= m; x++){
		PM[P[x-1]+127] = PM[P[x-1]+127] | (1 << (x-1));
	}
	////////////////////////////////////////////////////

	long VN = 0;
	long VP = 0xffffffff;
	long D0, HP, HN;

	long currDist = m;
	long currDistAnt;

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

			currDistAnt = currDist;

			match = true;

					quantGrupo++;
					acumulaPmedia += posicaoAtual;

		}else{
			if(match == true){
				indiceMedio = acumulaPmedia/quantGrupo;
				indexx.insert(indexx.begin(), (indiceMedio - m) + currDistAnt);
				break;
			}
		}

		if (posicaoAtual == n){
			if (match == true){
				indiceMedio = acumulaPmedia/quantGrupo;
				indexx.insert(indexx.begin(), (indiceMedio - m) + currDistAnt);
				break;
			}
		}

	}

	return indexx;

	}


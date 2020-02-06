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


//	char T[] = {"TTGACCTTCTTAGAAATATTCTTGATAAATGACCCCGGTTTTCTTGTAAAACCTTCTTGATGACCCTTCTTAGAAAATTGATGACCCTTAACGGCGACCACCTTCTTGATGACCCTTGACGACGGACCCA"};
//	char P[] = {"TTCTTGAAAA"};

	// char T[] = PP;
	// char P[] = TT;

//	long m = sizeof(P)/sizeof(char) - 1;
//	long n = sizeof(T)/sizeof(char) - 1;

//	K = NUMERO MAXIMO DE ERROS PERMITIDOS (DELECAO, SUBSTITUICAO, INSERCAO)
	// cerr << "K: " << k << endl;

	// ARMAZENA INDICES DE POSICOES CORRETAS DE MATCH APROXIMADO
	vector<int> indexx;
	// cerr << "M " << endl;

	long k = 3;

	// ===== VARIAVEIS RESPONSAVEIS NO PROCESSO DE DEFINIR INDICES CORRETOS DE MATCH NO CASAMENTO APROXIMADO
			// int inicializador = 0;
			int posicaoAnterior = 0;
			int quantGrupo = 0;
			int posicaoAtual = 0;
			int indiceMedio = 0;
			int acumulaPmedia = 0;
			bool match = false;
	// =======================================================================================================

	int MAXCHAR = 256;

	long Masc[MAXCHAR], i, j, Ri, Rant, Rnovo;
		// R[2]
	long R[k];

	for (i = 0; i < MAXCHAR; i++) Masc[i] = 0;

	for (i = 1; i <= m; i++){
			Masc[P[i-1]+127] |= 1 << (m-i);
			// cerr << "Masc P: " << Masc[P[i-1]+127] << endl;
			// cerr << "*****************" << endl;
		 } 
		 ///IMPRIMIR MASC

		R[0] = 0;
		Ri = 1 << (m-1); //10^(m-1)
		
		for (j = 1; j <= k; j++){
			R[j] = (1 << (m-j)) | R[j-1];
		}

		for (i = 0; i < n; i++){

			Rant = R[0];
			Rnovo = ((((unsigned long)Rant) >> 1) | Ri) & Masc[T[i] + 127];
			R[0] = Rnovo;

			for (j = 1; j <= k; j++){

				Rnovo = ((((unsigned long) R[j]) >> 1) & Masc[T[i] + 127]) | Rant | (((unsigned long)(Rant | Rnovo))  >> 1);
				Rant = R[j]; 
				R[j] = Rnovo | Ri; 

			}

			posicaoAtual = i+1;

			// SE VERDADEIRO - ACEITOU MATCH COM QUANTIDADE DE ERROS PERMITIDO
			if((Rnovo & 1) != 0){

				//cerr << "Casamento: " << posicaoAtual << endl;

			// ******* INICIO DE PROCESSO DE DEFINICOES DE INDICES CORRETOS DE MATCHS APROXIMADOS *******

				match = true;

			// INICIALIZA APENAS NO PRIMEIRO MATCH, PARA SALVAR HISTORICO DE POSICAO
				// if(inicializador == 1){

					//SOMA 1 AO GRUPO
					quantGrupo++;
					//POSICAO ANTERIOR ARMAZENA POSICAO ATUAL
					posicaoAnterior = posicaoAtual;
					//SOMA VALOR DE POSICAO AO GRUPO
					acumulaPmedia += posicaoAtual;

				// }
				// A PARTIR DO SEGUNDO MATCH
				// else{

				//se posicao anterior +1 eh igual a posicao atual (significa que eh parte do grupo)
				// if((posicaoAnterior+1) == posicaoAtual){

				// 		quantGrupo++;
				// 		posicaoAnterior = posicaoAtual;
				// 		acumulaPmedia += posicaoAtual;

				// }

				// else{

				// 	//TIRA A MEDIA DO GRUPO ANTERIOR - PEGANDO A SOMA ACUMULADA DO GRUPO DIVIDINDO-O PELA QUANTIDADE DE INDICES DO GRUPO 
				// 	//PARA SABER O VALOR EXATO DE MATCH
				// 	// indiceMedio = acumulaPmedia/quantGrupo;
				// 	// indexx.insert(indexx.begin(), (indiceMedio - m));
	
				// 	//cerr << "Casamento na posicao: " << indiceMedio << endl;

				// 	//ATRIBUI AO NOVO GRUPO SUCESSOR VALORES DE INICIALIZACAO - RECOMECA COM POSICAO ATUAL E QUANTIDADE DO GRUPO JA COM VALOR 1
				// 	acumulaPmedia = posicaoAtual;
				// 	posicaoAnterior = posicaoAtual;
				// 	quantGrupo = 1;
				// 	inicializador = 0;

				// 	// break;

				// }

		// }

			// PEGA TODOS OS MATCHS APROXIMADOS
			// indexx.push_back(i+1);

			}else{

				if(match == true){
					indiceMedio = acumulaPmedia/quantGrupo;
					indexx.insert(indexx.begin(), (indiceMedio - m));
					break;
				}

			}

			// SE FOR O ULTIMO CARACTER DA SEQUENCIA, EXIBI O ULTIMO MATCH CONSIDERADO
// 			if(posicaoAtual == n){

// 					if(quantGrupo >= 1){

// 						indiceMedio = acumulaPmedia/quantGrupo;

// //AQ						
// 						//cerr << "Casamento na posicaoZ: " << indiceMedio << endl;
// 						//indexx.push_back(indiceMedio);
// 						indexx.insert(indexx.begin(), (indiceMedio - m));


// 					}
// 			}

			// ************************************************************************************

		}

	 return indexx;

}

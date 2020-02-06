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
#include <string.h>

#define GRAM4(j) (B[y[j]] << 3) & (B[y[j - 1]] << 2) & (B[y[j - 2]] << 1) & B[y[j - 3]]

using namespace std;

vector<int> search(char *P, long m, char *T, long n){

// cerr << "Leitura" << endl;

//	char T[] = {"TTGACCTTCTTAGAAATATTCTTGATAAATGACCCCGGTTTTCTTGTAAAACCTTCTTGATGACCCTTCTTAGAAAATTGATGACCCTTAACGGCGACCACCTTCTTGATGACCCTTGACGACGGACCCA"};
//	char P[] = {"TTCTTGAAAA"};

	// char T[] = PP;
	// char P[] = TT;

//	long m = sizeof(P)/sizeof(char) - 1;
//	long n = sizeof(T)/sizeof(char) - 1;

//	K = NUMERO MAXIMO DE ERROS PERMITIDOS (DELECAO, SUBSTITUICAO, INSERCAO)
	long k = 3;
	// cerr << "K: " << k << endl;

	// ARMAZENA INDICES DE POSICOES CORRETAS DE MATCH APROXIMADO
	vector<int> indexx;
	// cerr << "M " << endl;


	// ===== VARIAVEIS RESPONSAVEIS NO PROCESSO DE DEFINIR INDICES CORRETOS DE MATCH NO CASAMENTO APROXIMADO
			int inicializador = 0;
			int posicaoAnterior = 0;
			int quantGrupo = 0;
			int posicaoAtual = 0;
			int indiceMedio = 0;
			int acumulaPmedia = 0;
	// =======================================================================================================
 
            int inicializadorB = 0;
            int posicaoAnteriorB = 0; 
            int quantGrupoB = 0;
            int indiceMedioB = 0;
            int posicaoAtualB = 0;
            int acumulaPmediaB = 0; 

            bool sA = false; 
            bool sB = false; 

			int MAXCHAR = 256;

			int meioPalavra = n / 2;

	unsigned long Masc[MAXCHAR], i, j, Ri, Rant, Rnovo;
		// R[2]
	unsigned long R[k];

	for (i = 0; i < MAXCHAR; i++) Masc[i] = 0;

	// máscara de bits de Prefixos de caracteres 
	for (i = 1; i <= m; i++){

			Masc[P[i-1]+127] |= 1 << (m-i);
			Masc[P[i-1]+127] &= 0xffffffff;
			// cerr << "I: " << i << endl;
			// cerr << "-: " << Masc[P[i-1]+127] << endl;
	}

		R[0] = 0;

		Ri = 0x8000000080000000; // 10^m-1 Duplicado OK

		// máscara de mismatchs
		for (j = 1; j <= k; j++){

			R[j] = (1 << (m-j)) | R[j-1];
			//*
			R[j] = (R[j] << m) | R[j];

		}
        
//+(m-1)
		for (i = 0; i < (meioPalavra); i++){

			Rant = R[0];

			// R' *
			unsigned long pt2 = Masc[T[i+meioPalavra] + 127];

			// if (i >= meioPalavra) pt2 &= 0;

			Rnovo = ((((unsigned long)Rant) >> 1) | Ri) & ((Masc[T[i] + 127] << m) | pt2);

			R[0] = Rnovo;

			//Correndo na máscara de Mismatchs
			for (j = 1; j <= k; j++){

				// pt2 = Masc[T[i+meioPalavra] + 127];
				
				// if (i >= meioPalavra) pt2 &= 0;

				unsigned long doubleMask = Masc[T[i] + 127] << m | pt2;
				Rnovo = ((((unsigned long) R[j]) >> 1) & doubleMask) | Rant | (((unsigned long)(Rant | Rnovo)) >> 1);
				Rant = R[j];
				R[j] = Rnovo | Ri;

			}

			// cerr << "--------------------------------------" << endl;
			// cerr << "Rnovo: " << Rnovo << endl;

			posicaoAtual = i+1;
			posicaoAtualB = i+meioPalavra+1;

			unsigned long sideA = Rnovo >> m; // OK
            unsigned long sideB = 0xffffffff & Rnovo; // OK

			// cerr << "sideA: " << sideA << endl;
			// cerr << "sideB: " << sideB << endl;

			// SE VERDADEIRO - ACEITOU MATCH COM QUANTIDADE DE ERROS PERMITIDO
			 if((sideA & 1) != 0){ 

		    	// cerr << "Entrei A" << endl;

                sA = true; 

			// ******* INICIO DE PROCESSO DE DEFINICOES DE INDICES CORRETOS DE MATCHS APROXIMADOS *******

				inicializador++;

			// INICIALIZA APENAS NO PRIMEIRO MATCH, PARA SALVAR HISTORICO DE POSICAO
				if(inicializador == 1){

					//SOMA 1 AO GRUPO
					quantGrupo++;
					//POSICAO ANTERIOR ARMAZENA POSICAO ATUAL
					posicaoAnterior = posicaoAtual;
					//SOMA VALOR DE POSICAO AO GRUPO
					acumulaPmedia += posicaoAtual;

				}
				// A PARTIR DO SEGUNDO MATCH
				else{

				//se posicao anterior +1 eh igual a posicao atual (significa que eh parte do grupo)
				if((posicaoAnterior+1) == posicaoAtual){

						quantGrupo++;
						posicaoAnterior = posicaoAtual;
						acumulaPmedia += posicaoAtual;

				}else{

					//TIRA A MEDIA DO GRUPO ANTERIOR - PEGANDO A SOMA ACUMULADA DO GRUPO DIVIDINDO-O PELA QUANTIDADE DE INDICES DO GRUPO 
					//PARA SABER O VALOR EXATO DE MATCH
					indiceMedio = acumulaPmedia/quantGrupo;
					indexx.insert(indexx.begin(), (indiceMedio - m));
					// return indexx;
	
					//cerr << "Casamento na posicao: " << indiceMedio << endl;

					//ATRIBUI AO NOVO GRUPO SUCESSOR VALORES DE INICIALIZACAO - RECOMECA COM POSICAO ATUAL E QUANTIDADE DO GRUPO JA COM VALOR 1
					acumulaPmedia = posicaoAtual;
					posicaoAnterior = posicaoAtual;
					quantGrupo = 1;

				}

		}

			// PEGA TODOS OS MATCHS APROXIMADOS
			// indexx.push_back(i+1);

			}

			if((sideB & 1) != 0){

                sB = true; 
                // cerr << "Entrei B" << endl;

			// ******* INICIO DE PROCESSO DE DEFINICOES DE INDICES CORRETOS DE MATCHS APROXIMADOS *******

				inicializadorB++;

			// INICIALIZA APENAS NO PRIMEIRO MATCH, PARA SALVAR HISTORICO DE POSICAO
				if(inicializadorB == 1){

					//SOMA 1 AO GRUPO
					quantGrupoB++;
					//POSICAO ANTERIOR ARMAZENA POSICAO ATUAL
					posicaoAnteriorB = posicaoAtualB;
					//SOMA VALOR DE POSICAO AO GRUPO
					acumulaPmediaB += posicaoAtualB;

				}
				// A PARTIR DO SEGUNDO MATCH
				else{

				//se posicao anterior +1 eh igual a posicao atual (significa que eh parte do grupo)
				if((posicaoAnteriorB+1) == posicaoAtualB){

						quantGrupoB++;
						posicaoAnteriorB = posicaoAtualB;
						acumulaPmediaB += posicaoAtualB;

				}else{

					//TIRA A MEDIA DO GRUPO ANTERIOR - PEGANDO A SOMA ACUMULADA DO GRUPO DIVIDINDO-O PELA QUANTIDADE DE INDICES DO GRUPO 
					//PARA SABER O VALOR EXATO DE MATCH
					indiceMedioB = acumulaPmediaB/quantGrupoB;
					indexx.insert(indexx.begin(), (indiceMedioB - m));
					// return indexx;
	
					//cerr << "Casamento na posicao: " << indiceMedio << endl;

					//ATRIBUI AO NOVO GRUPO SUCESSOR VALORES DE INICIALIZACAO - RECOMECA COM POSICAO ATUAL E QUANTIDADE DO GRUPO JA COM VALOR 1
					acumulaPmediaB = posicaoAtualB;
					posicaoAnteriorB = posicaoAtualB;
					quantGrupoB = 1;

				}

		}

			// PEGA TODOS OS MATCHS APROXIMADOS
			// indexx.push_back(i+1);

			}

			// SE FOR O ULTIMO CARACTER DA SEQUENCIA, EXIBI O ULTIMO MATCH CONSIDERADO
			if(posicaoAtual == meioPalavra){

					if(sA == true){

                    if(quantGrupo >= 1){

                        indiceMedio = acumulaPmedia/quantGrupo; 
                        indexx.insert(indexx.begin(), (indiceMedio - m)); 
                        // return indexx;
  
                   }
               }

                    if(sB == true){

                    if(quantGrupoB >= 1){  

                        indiceMedioB = acumulaPmediaB/quantGrupoB; 
                        indexx.insert(indexx.begin(), (indiceMedioB - m)); 
                        // return indexx;

                    } 

                } 
			// ************************************************************************************
		}
	}

	 return indexx;

}

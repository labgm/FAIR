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

	int MAXCHAR = 256, limitInf, limitSup;

    long MascPint[MAXCHAR], Masc[MAXCHAR], i, j, ii, jj, Ri, Rant, Rnovo, Rant2, Rnovo2;

	// CRIA PADRÃO INTERCALADO -> Pint
	int QuantGroup = k + 1; 
    int r = m / QuantGroup;
   	std::string Pint;
    for(int ii = 0; ii <= (r-1); ii++){
        int x = ii;	
        for(int jj = 0; jj <= k; jj++){ 
            Pint += P[x];
            x += r;
        }
     }

// ///////// PRÉ-PROCESSAMENTO DO CASAMENTO APROXIMADO /////////////////////////////////
    int inicializador, posicaoAnterior, quantGrupo, posicaoAtual, indiceMedio, acumulaPmedia;
	// MÁSCARA DE BITS DO PADRÃO NORMAL
	for (ii = 0; ii < MAXCHAR; ii++) Masc[ii] = 0;
	for (ii = 1; ii <= m; ii++) Masc[P[ii-1]+127] |= 1 << (m-ii);

		Ri = 1 << (m-1); //10^(m-1)
		long R2[k];
		R2[0] = 0;
		
		//MÁSCARA DE MISMATCH
		for (jj = 1; jj <= k; jj++) R2[jj] = (1 << (m-jj)) | R2[jj-1];
///////////////////////////////////////////////////////////////////////////////////////

/////// PRÉ-PROCESSAMENTO CASAMENTO EXATO ///////////////////////////////////////////////
     //MÁSCARA DE BITS DO PADRÃO INTERCALADO
	for (i = 0; i < MAXCHAR; i++) MascPint[i] = 0;
	for (i = 1; i <= m; i++) MascPint[Pint[i-1]+127] |= 1 << (m-i);

		long Rix = 0xF0000000;
		long R[k];
		R[0] = 0;
///////////////////////////////////////////////////////////////////////////////////////////
	for (i = 0; i < n; i++){

			Rant = R[0];
			Rnovo = ((((unsigned long)Rant) >> QuantGroup) | Rix) & MascPint[T[i] + 127];
			R[0] = Rnovo;

			//SABENDO SE ÚLTIMOS (k+1) bits estão ativos
			// int z = 0xF;
			// for (ii = 0; ii < k; ++ii) z = (z << 1) | 1;

	// IF VERDADEIRO ENCONTROU MATCH EXATO
	if ((0xF & Rnovo) > 0){
	
	// ===== VARIAVEIS RESPONSAVEIS NO PROCESSO DE DEFINIR INDICES CORRETOS DE MATCH NO CASAMENTO APROXIMADO
			inicializador = 0;
			posicaoAnterior = 0;
			quantGrupo = 0;
			posicaoAtual = 0;
			indiceMedio = 0;
			acumulaPmedia = 0;
	// =======================================================================================================
		limitInf = i - 9;
		limitSup = i + m + QuantGroup;

		for (i = limitInf; i < limitSup; i++){

			Rant2 = R2[0];
			Rnovo2 = ((((unsigned long)Rant2) >> 1) | Ri) & Masc[T[i] + 127];
			R2[0] = Rnovo2;

			for (jj = 1; jj <= k; jj++){

				Rnovo2 = ((((unsigned long) R2[jj]) >> 1) & Masc[T[i] + 127]) | Rant2 | (((unsigned long)(Rant2 | Rnovo2)) >> 1);
				Rant2 = R2[jj]; 
				R2[jj] = Rnovo2 | Ri; 

			}

			posicaoAtual = i+1;

			// SE VERDADEIRO - ACEITOU MATCH COM QUANTIDADE DE ERROS PERMITIDO
			if((Rnovo2 & 1) != 0){
				inicializador++;
				if(inicializador == 1){
					quantGrupo++;
					posicaoAnterior = posicaoAtual;
					acumulaPmedia += posicaoAtual;
				}else{
				//se posicao anterior +1 eh igual a posicao atual (significa que eh parte do grupo)
				if((posicaoAnterior+1) == posicaoAtual){
						quantGrupo++;
						posicaoAnterior = posicaoAtual;
						acumulaPmedia += posicaoAtual;
				}

				else{

								indiceMedio = acumulaPmedia/quantGrupo;
								indexx.insert(indexx.begin(), (indiceMedio - m));

					acumulaPmedia = posicaoAtual;
					posicaoAnterior = posicaoAtual;

					quantGrupo = 1;
					inicializador = 1;
					
					R[0] = 0;
					R2[0] = 0;

			}

		}
	}


			// SE FOR O ULTIMO CARACTER DA SEQUENCIA, EXIBI O ULTIMO MATCH CONSIDERADO
			if(posicaoAtual == limitSup){

					if(quantGrupo >= 1){
						indiceMedio = acumulaPmedia/quantGrupo;
						indexx.insert(indexx.begin(), (indiceMedio - m));
					}

			}

		}

		R[0] = 0;
		R2[0] = 0;

			}

		}

// cerr << "--------------------------------------------------------------------------------" << endl;

	return indexx;

}
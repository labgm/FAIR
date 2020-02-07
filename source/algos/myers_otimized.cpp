// MYERS (99) MODIFICADO POR HYRRO E NAVARRO (2005)
// CAP 4. SEARCHING FOR SEVERAL PATTERNS SIMULTANEOUSLY

//BUSCA O PADRÃO EM TODA A SEQUENCIA

//FALTANDO TERMINAR ALGORITMO

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

#include <math.h>

using namespace std;

vector<int> search(char *PP, long m, char *T, long n){

	char P[64];

	strcpy(P, PP);
	strcat(P, PP);

	// cerr << P << endl;

	long k = 3;
	
	vector<int> indexx;

	int MAXCHAR = 256;


	// ComputePM ////////////////////////////////////////
	long PM[MAXCHAR];
	for (int ii = 0; ii < MAXCHAR; ii++) PM[ii] = 0;
	for (int ii = 1; ii <= 64; ii++){
		long temp = 1 << (ii-1);
		PM[P[ii-1]+127] = PM[P[ii-1]+127] | temp;
	}
	////////////////////////////////////////////////////


	// MComputePM //////////////////////////////////////// x
	// long PM[MAXCHAR];
	for (int x = 0; x < MAXCHAR; x++) PM[x] = 0;

	int w = 64;
	int r = w / m;
	for (int s = 1; s <= 1; s++){
		for (int i = 1; i <= m; ++i){
			long constt = 0 << ((r-s+1) * m ) - i;
			long ccc = 1 << (m*(s-1)+i-1);
			long constt2 = (constt << ccc) | ccc;
			PM[P[i-1]+127] = PM[P[i-1]+127] | constt2;
		}
	}
	////////////////////////////////////////////////////

	long ZM;
	ZM = 0x7FFFFFFF7FFFFFFF;
	// ZM = (ZM << m) | ZM;

	long EM;
	EM = 0x8000000080000000;
	// EM = (EM << m) | EM;

	long VN = 0x0000000000000000;

	long VP = 0xFFFFFFFFFFFFFFFF;

	long tempp = pow(2, (m-1));
	long MC = (tempp + k) * ZM;

	long D0, HP, HN, XP, XN;

	for (int j = 1; j <= n; ++j){	

		// MStep ////////////////////////////////////////////////// OK
		XP = VP & ZM;
		D0 = (((PM[T[j-1]+127] & XP) + XP) ^ XP) | PM[T[j-1]+127] | VN;
		HP = VN | ~ (D0 | VP);
		HN = VP & D0;
		XP = (HP & ZM) << 1;
		XN = (HN & ZM) << 1;
		VP = (XN | ~ (D0 | XP));
		VN = XP & D0;
		//////////////////////////////////////////////////////////

		MC = MC + ((HN & EM) >> (m-1)) - ((HP & EM) >> (m-1));

		long OM = MC & EM;
		if (OM != 0x0000000000000000){

			while (OM != 0x0000000000000000){
				
				int s = log2(OM);
				int position = (s+1)/m;
				cerr << "Ocorrência > " << P[position] << endl;
				cerr << "POSICAO > " << j << endl;
				OM = OM & ( ~ (1 << s));

			}

		}

	}

	cerr << "========================================================================================" << endl;

	return indexx;

	}


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

	// cerr << P << "B" << endl;
	// cerr << "Z" << endl;

	unsigned long k = 3;
	
	vector<int> indexx;

	int MAXCHAR = 256;


	// ComputePM ////////////////////////////////////////
	unsigned long PM[MAXCHAR];
	// for (int x = 0; x < MAXCHAR; x++) PM[x] = 0;
	// for (int x = 1; x <= m; x++){
	// 	PM[P[x-1]+127] = PM[P[x-1]+127] | (1 << (x-1));
	// }
	// ////////////////////////////////////////////////////


	// MComputePM //////////////////////////////////////// x
	// long PM[MAXCHAR];
	for (int x = 0; x < MAXCHAR; x++) PM[x] = 0;

	int w = 64;
	int r = w / m;
	long constt, test, ccc, constt2;
	for (int s = 1; s <= r; s++){
		for (int i = 1; i <= w; ++i){
			constt = 0 << ((r-s+1) * m ) - i;
			test = ((m*(s-1))+i-1);
			ccc = 1 << ((m*(s-1))+i-1);
			constt2 = (constt << (test + 1)) | ccc;
			PM[P[i-1]+127] = PM[P[i-1]+127] | constt2;
		}
	}
	////////////////////////////////////////////////////

	unsigned long ZM;
	ZM = 0x7FFFFFFF7FFFFFFF; //ok

	unsigned long EM;
	EM = 0x8000000080000000; //ok

	unsigned long VN = 0; //ok

	unsigned long VP = 0xFFFFFFFFFFFFFFFF; //ok

	unsigned long tempp = pow(2, (m-1)); //ok

	unsigned long MC = (tempp + k) * (0x0000000100000001); //ok

	unsigned long D0, HP, HN, XP, XN;

	for (int j = 0; j < n; ++j){	

		// MStep ////////////////// OK //////////////////////////////// OK
		XP = VP & ZM;
		D0 = (((PM[T[j]+127] & XP) + XP) ^ XP) | PM[T[j]+127] | VN;
		HP = VN | ~ (D0 | VP);
		HN = VP & D0;
		XP = (HP & ZM) << 1;
		XN = (HN & ZM) << 1;
		VP = (XN | ~ (D0 | XP));
		VN = XP & D0;
		//////////////////////////////////////////////////////////

		MC += ((HN & EM) >> (m-1)) - ((HP & EM) << (m-1));

		unsigned long OM = MC & EM;
		if (OM != 0){

			while (OM != 0){
				
				int s = log2(OM);
				cerr << "s > " << s << endl;
				int myP = (s+1)/m;
				cerr << "P > " << myP << endl;
				cerr << "POSICAO > " << j << endl;
				OM = OM & ( ~ (1 << s));

			}

		}

	}

	cerr << "========================================================================================" << endl;

	return indexx;

	}


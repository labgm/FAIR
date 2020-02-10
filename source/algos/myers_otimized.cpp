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

	char conc[33];

	cerr << "SIZE T > " << strlen(T) << endl;

	strcpy(conc, "CAGGTCGTAACTGTGGTCAGCCGAGTTAAGCG");

	strcpy(P, PP);
	strcat(P, conc);

	cerr << P << endl;
	// cerr << "Len " << strlen(P) << endl;

	unsigned long k = 0;
	
	vector<int> indexx;

	int MAXCHAR = 256;


	// MComputePM //////////////////////////////////////// x
	long PM[MAXCHAR];
	for (int x = 0; x < MAXCHAR; x++) PM[x] = 0x0000000000000000;

	int w = 64;
	int r = w / m;
	long constt;
	for (int s = 1; s <= r; s++){
		for (int i = 1; i <= m; ++i){
			constt = 1 << ((m*(s-1))+i-1);			
			PM[P[i-1]+127] = PM[P[i-1]+127] | constt;
		}
	}
	////////////////////////////////////////////////////

	long ZM;
	ZM = 0x7FFFFFFF7FFFFFFF; //ok

	long EM;
	EM = 0x8000000080000000; //ok

	long VN = 0x0000000000000000; //ok

	long VP = 0xFFFFFFFFFFFFFFFF; //ok

	unsigned long temp = pow(2, m-1);
	unsigned int MC = (temp + k) * (0x0000000100000001);
//	 MC = (2147483648 + k) * 4294967297;	 	


	long D0, HP, HN, XP, XN;
	unsigned long OM; 

	for (int j = 1; j <= n; j++){

		// MStep ////////////////// OK //////////////////////////////// OK
		XP = VP & ZM;
		D0 = (((PM[T[j-1]+127] & XP) + XP) ^ XP) | PM[T[j-1]+127] | VN;
		HP = VN | ~ (D0 | VP);
		HN = VP & D0;
		XP = (HP & ZM) << 1;
		XN = (HN & ZM) << 1;

		VP = (XN | ~ (D0 | XP));
		VN = XP & D0;
		//////////////////////////////////////////////////////////

		MC = MC + (((HN & EM) >> (m-1)) - ((HP & EM) >> (m-1)));

		OM = MC & EM;
		if (OM != 0x0000000000000000){

				cerr << "MATCH > " << j << endl;

			while (OM != 0x0000000000000000){
				
				long s = log2(OM);

				cerr << "Padrão: " << s << endl;
				OM = OM & ( ~ (1 << s));
				// cerr << "OM 2 > " << OM << endl;

			}

		}

	}

	cerr << "========================================================================================" << endl;

	return indexx;

	}


// MYERS (99) MODIFICADO POR HYRRO E NAVARRO (2005)
// CAP 4. SEARCHING FOR SEVERAL PATTERNS SIMULTANEOUSLY

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

#include <math.h>

using namespace std;

void MReport(int j, unsigned long OM){

			while (OM != 0x0000000000000000){
				
				int s = floor(log2(OM));
				cerr << "s = " << s << endl;
				cerr << "MATCH = " << j << endl;
				cerr << "OM = " << OM << endl;
				OM = OM & (~(1 << s));
				cerr << "-----------------" << endl;

			}

}

vector<int> search(char *PP, long m, char *T, long n){

	char P[64];

	char conc[33];

	// cerr << "SIZE T > " << strlen(T) << endl;

	strcpy(conc, "ATATATATATATATATATATATATATATATAT");

	strcpy(P, conc);
	strcat(P, PP);

	cerr << P << endl;

	unsigned long k = 0;
	
	vector<int> indexx;

	int MAXCHAR = 256;

	// MComputePM //////////////////////////////////////// x
	unsigned long PM[MAXCHAR];
	for (int x = 0; x < MAXCHAR; x++) PM[x] = 0;

	int w = 64;
	// int r = w / m;
	int r = 2;
	unsigned long constt, constt2, tmp;
	for (int s = 1; s <= r; s++){
		for (int i = 1; i <= m; i++){
			// constt2 = m*(r-s+1)-1;
			constt = 1 << (m*(s-1)+i-1);	
			cerr << "constt: " << constt << endl;
			// tmp = (0 << constt2) | constt;
			// tmp = constt;
			// cerr << "tmp > " << tmp << endl;
			PM[P[i-1]] = PM[P[i-1]] | constt;
		}
	}

	////////////////////////////////////////////////////

	unsigned long ZM = 0x7FFFFFFF7FFFFFFF; //ok
	unsigned long EM = 0x8000000080000000; //ok
	unsigned long VN = 0; //ok
	unsigned long VP = 0xFFFFFFFFFFFFFFFF; //ok

	unsigned long temp = pow(2, m-1);
	unsigned long long MC = (temp + k) * (0x0000000100000001);
	// unsigned long long MC = (temp + k) * 4294967297;
//	MC = (2147483648 + k) * 4294967297;	 	


	unsigned long D0 = 0, HP = 0, HN = 0, XP = 0, XN = 0;

	for (int j = 1; j <= n; j++){

		cerr << "Carac: " << T[j-1] << endl;
		cerr << "MC 1: " << MC << endl;
		// MStep ////////////////// OK //////////////////////////////// OK
		XP = VP & ZM;
		D0 = (((PM[T[j-1]] & XP) + XP) ^ XP) | PM[T[j-1]] | VN;
		HP = VN | ~(D0 | VP);
		HN = VP & D0;
		XP = (HP & ZM) << 1;
		XN = (HN & ZM) << 1;

		VP = (XN | ~(D0 | XP));
		VN = XP & D0;
		//////////////////////////////////////////////////////////

		MC = MC + ((HN & EM) >> (m-1)) - ((HP & EM) >> (m-1));
		cerr << "MC 2: " << MC << endl;
		
		unsigned long tp = MC & EM;
		
		if (tp != 0x0000000000000000){
			MReport(j, tp);		

		}

	}

	cerr << "========================================================================================" << endl;

	return indexx;

	}

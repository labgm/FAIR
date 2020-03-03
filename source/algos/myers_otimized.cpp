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
				OM = OM & (~ (1 << s));
				cerr << "-----------------" << endl;

			}

}

vector<int> search(char *PP, long m, char *T, long n){

	char P[64];

	char conc[33];

	// cerr << "SIZE T > " << strlen(T) << endl;

	strcpy(conc, "CAGGTCGTAACTGTGGTCAGCCGAGTTAAGCG");

	strcpy(P, conc);
	strcat(P, PP);

	// cerr << P << endl;

	unsigned long k = 0;
	
	vector<int> indexx;

	int MAXCHAR = 256;

	// MComputePM //////////////////////////////////////// x
	unsigned long PM[MAXCHAR];
	for (int x = 0; x < MAXCHAR; x++) PM[x] = 0x0000000000000000;

	int w = 64;
	// int r = w / m;
	int r = 2;
	unsigned long constt, constt2, tmp;
	for (int s = 1; s <= r; s++){
		for (int i = 1; i <= m; i++){
			// constt2 = m*(r-s+1)-1;
			constt = 1 << (m*(s-1)+i-1);	
			// tmp = (0 << constt2) | constt;

			tmp = constt;
			// cerr << "tmp > " << tmp << endl;
			PM[P[i-1]+127] = PM[P[i-1]+127] | tmp;
		}
	}

	////////////////////////////////////////////////////

	unsigned long ZM;
	ZM = 0x7FFFFFFF7FFFFFFF; //ok

	unsigned long EM;
	EM = 0x8000000080000000; //ok

	unsigned long VN = 0; //ok

	unsigned long VP = 0xFFFFFFFFFFFFFFFF; //ok

	unsigned long temp = pow(2, m-1);
	unsigned long MC = (temp + k) * (0x0000000100000001);
//	MC = (2147483648 + k) * 4294967297;	 	

	unsigned long D0 = 0, HP = 0, HN = 0, XP = 0, XN = 0;

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

		MC = MC + ((HN & EM) >> (m-1)) - ((HP & EM) >> (m-1));
		
		unsigned long tp = MC & EM;
		
		if (tp != 0x0000000000000000){
			MReport(j, tp);		

		}

	}

	cerr << "========================================================================================" << endl;

	return indexx;

	}

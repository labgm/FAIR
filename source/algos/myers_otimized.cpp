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
#include <stdint.h>
#include <math.h>

using namespace std;

// REPORT OCCURRENCE PATTERN
void MReport(int j, uintmax_t OM){

	while (OM != 0){
		
		int s = floor(log2(OM));
		cerr << "\n-----------------" << endl;
		// cerr << "s = " << s << endl;
		cerr << "MATCH = " << j << endl;
		cerr << "OM1 = " << OM << endl;
		OM = OM & ~(1 << s);
		cerr << "OM2 = " << OM << endl;
		cerr << "-----------------\n" << endl;

	}

}

vector<int> search(char *PP, long m, char *T, long n){

	char P[65];

	char conc[33];

	// cerr << "SIZE T > " << strlen(T) << endl;

	// strcpy(conc, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	strcpy(conc, "CAGGTCGTAACTGTGGTCAGCCGAGTTAAGCG");

	strcpy(P, conc);
	strcat(P, PP);

	cerr << "Padrão: " << P << endl;
	// cerr << T << endl;

	int k = 0;
	
	vector<int> indexx;

	int MAXCHAR = 1024;

	// MComputePM //////////////////////////////////////// x
	uintmax_t PM[MAXCHAR];
	for (int x = 1; x <= MAXCHAR; x++) PM[x-1] = (0x0000000000000000);

	// for (int x = 0; x < MAXCHAR; x++) cerr << "-:" << PM[x] << endl;	

	int w = 64;
	int r = w / m;
	// int r = 2;
	uintmax_t constt, constt2, tmp;

	for (int s = 1; s <= r; s++){
		for (int i = 1; i <= m; i++){
			// constt2 = m*(r-s+1)-i;
			uint64_t um = 1;
			constt = um << m*(s-1)+i-1;	// ok
			cerr << "constt: " << constt << endl;
			// tmp = (UINTMAX_C(0) << constt2) | constt;
			// tmp = constt;
			// cerr << "tmp > " << tmp << endl;
			PM[P[m*(s-1)+i-1]] = constt | PM[P[m*(s-1)+i-1]];
				cerr << "Pos. Caracter P > " << (m*(s-1)+i-1) << endl;		
				cerr << "Mascara de bits: " << PM[P[m*(s-1)+i-1]] << endl; //ok
				cerr << "--------------------" << endl;
		}
	}

	// for (uintmax_t ii = 1; ii <= 64; ii++){
	// 	uintmax_t tmpy = UINTMAX_C(1) << (ii-1);
	// 	cerr << ii-1 << ":" << tmpy << endl;
	// 	PM[P[ii-1]] = PM[P[ii-1]] | tmpy;
	// 	// cerr << ii-1 << ": " << PM[P[ii-1]] << endl;
	// }

	// for (int i = 0; i < 64; ++i)
	// {
	// 	cerr << i << "# " << PM[P[i-1]] << endl;
	// }

	////////////////////////////////////////////////////

	uintmax_t ZM = (0x7FFFFFFF7FFFFFFF);
	// uintmax_t ZM = UINTMAX_C(0xFFFFFFFEFFFFFFFE); //reverso
	uintmax_t EM = (0x8000000080000000); 
//    uintmax_t EM2 = UINTMAX_C(0x0000000100000001); //reverso
	uintmax_t VN = (0x0000000000000000); 
	uintmax_t VP = (0xFFFFFFFFFFFFFFFF); 

	int temp = pow(2, m-1);
	uintmax_t MC = (temp + k) * (0x0000000100000001);
	// cerr << "MC1: " << MC << endl;
	// uintmax_t MC = (temp + k) * UINTMAX_C(0x8000000080000000); //reverso
	// uintmax_t long MC = (temp + k) * 4294967297;
//	MC = (2147483648 + k) * 4294967297;	 	


	uintmax_t D0, HP, HN, XP, XN;

	for (int j = 1; j <= n; j++){

		// cerr << "Carac: " << T[j-1] << endl;
		// cerr << "J>>>>>>>> " << j-1 << endl;

		// MStep ////////////////// OK ///////////////////////////		
		XP = VP & ZM;
		// cerr << "XP: " << XP << endl;
		D0 = (((PM[T[j-1]] & XP) + XP) ^ XP) | PM[T[j-1]] | VN;
		// cerr << "D0: " << D0 << endl;
		HP = VN | ~(D0 | VP);
		// cerr << "HP: " << HP << endl;
		HN = VP & D0;
		// cerr << "HN: " << HN << endl;
		XP = (HP & ZM) << 1;
		// cerr << "XP: " << XP << endl;
		XN = (HN & ZM) << 1;
		// cerr << "XN: " << XN << endl;

		VP = (XN | ~(D0 | XP));
		// cerr << "VP: " << VP << endl;
		VN = XP & D0;
		// cerr << "VN: " << VN << endl;
		//////////////////////////////////////////////////////////

		MC = MC + ((HN & EM) >> (m-1)) - ((HP & EM) >> (m-1));
		// cerr << "MC: " << MC << endl;
		// cerr << "MC 2: " << MC << endl;
		
		uint64_t tp = MC & EM;
		cerr << "tp: " << tp << endl;
		
		if (tp != 0x0000000000000000){
			// MReport(j, tp);		
			cerr << "MATCH: " << j-1 << endl;
		}

	}

	cerr << "=================================================" << endl;

	return indexx;

	}
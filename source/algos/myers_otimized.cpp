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

void MReport(int j, unsigned long long OM)
{
	cerr << "--x--" << endl;
	while (OM != 0)
	{
		int s = log2(OM);
		// cerr << "s > " << s << endl;
		int ind = (s + 1) / 32;
		cerr << "Padrao (" << ind << ")" << endl;
		cerr << "MATCH > " << j << endl;
		unsigned long long ola = (0x1ULL << s);
		OM = OM & ~ola;
	}
	cerr << "--y--" << endl;
}

vector<int> searchMyers(char *PP, long m, char *T, long n, int mismatchMax, int indiceStart)
{

	char P[64];

	char conc[33];

	cerr << "SIZE T > " << strlen(T) << endl;

	strcpy(conc, "CGCGCGCGCGCGCGCGCGCGCGCGCGCGCGCG"); // ATATATATATATATATATATATATATATATAT

	strcpy(P, conc);
	strcat(P, PP);

	cerr << P << endl;

	unsigned long long k = mismatchMax;

	vector<int> indexx;

	int MAXCHAR = 512;

	// MComputePM //////////////////////////////////////// x
	unsigned long long PM[MAXCHAR];
	for (int x = 0; x < MAXCHAR; x++)
		PM[x] = 0x0000000000000000ULL;

	int w = 64;
	int r = w / m;
	unsigned long long constt, constt2, tmp;
	int contadorr = 1;
	for (int s = 1; s <= r; s++)
	{
		for (int i = 1; i <= m; i++)
		{
			// cerr << "i: " << i << endl; // i -> 1 à 32 2x
			constt2 = m * (r - s + 1) - 1;
			constt = 1ULL << (m * (s - 1) + i - 1);
			tmp = (0 << constt2) | constt;
			// cerr << "tmp > " << tmp << endl;
			PM[P[contadorr - 1] + 127] = PM[P[contadorr - 1] + 127] | tmp;
			// cerr << contadorr << ": " << PM[P[contadorr - 1] + 127] << endl;
			contadorr++;
		}
	}

	////////////////////////////////////////////////////

	// Using for boundary condition
	unsigned long long ZM;
	ZM = 0x7FFFFFFF7FFFFFFFULL; //ok

	// cerr << "ZM: " << ZM << endl;

	unsigned long long EM;
	EM = 0x8000000080000000ULL; //ok
	// cerr << "EM: " << EM << endl;

	unsigned long long VN = 0x0ULL; //ok
	// cerr << "VN: " << VN << endl;

	unsigned long long VP = 0xFFFFFFFFFFFFFFFFULL; //ok
	// cerr << "VP: " << VP << endl;

	unsigned long long temp = pow(2, m - 1);
	unsigned long long MC = (temp + k) * (0x0000000100000001ULL);
	// cerr << "MC(0): " << MC << endl;
	//	 MC = (2147483648 + k) * 4294967297;

	unsigned long long D0 = 0x0ULL, HP = 0x0ULL, HN = 0x0ULL, XP = 0x0ULL, XN = 0x0ULL;

	for (int j = 0; j < n; j++)
	{
		cerr << "J: " << j << endl;

		// MStep ////////////////// OK //////////////////////////////// OK
		// cerr << "----" << endl;
		XP = VP & ZM;
		// cerr << "XP: " << XP << endl;
		// cerr << "PMTJ: " << PM[T[j - 1] + 127] << endl;
		D0 = (((PM[T[j] + 127] & XP) + XP) ^ XP) | PM[T[j] + 127] | VN;
		// cerr << "D0: " << D0 << endl;
		HP = VN | ~(D0 | VP);
		// cerr << "HP: " << HP << endl;
		HN = VP && D0;
		// cerr << "HN: " << HN << endl;
		XP = (HP && ZM) << 1;
		// cerr << "XP: " << XP << endl;
		XN = (HN && ZM) << 1;
		// cerr << "XN: " << XN << endl;

		VP = (XN | ~(D0 | XP));
		// cerr << "VP: " << VP << endl;
		VN = XP & D0;
		// cerr << "VN: " << VN << endl;
		//////////////////////////////////////////////////////////

		cerr << "MC 1: " << MC << endl;

		MC = MC + ((HN & EM) >> (m - 1)) - ((HP & EM) >> (m - 1));

		unsigned long long RE;
		RE = MC & EM;
		cerr << "MC 2: " << MC << endl;

		cerr << "RE: " << RE << endl;
		// cerr << "----\n" << endl;
		if (RE != 0)
		{
			MReport(j, RE);
		}
	}

	cerr << "========================================================================================" << endl;

	return indexx;
}

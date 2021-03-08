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

using namespace std;

// Myers (1999)
vector<int> searchMyers(char *P, long m, char *T, long n, int mismatchMax, int indiceStart)
{

	int k = mismatchMax;

	vector<int> indexx;

	int MAXCHAR = 256;
	int posicaoAnterior, quantGrupo, posicaoAtual, indiceMedio;

	int inicializador = 0;
	posicaoAnterior = 0;
	quantGrupo = 0;
	posicaoAtual = 0;
	indiceMedio = 0;

	// ComputePM ////////////////////////////////////////
	unsigned long long PM[MAXCHAR];
	for (int ii = 0; ii < MAXCHAR; ii++)
		PM[ii] = 0L;
	for (int ii = 1; ii <= m; ii++)
	{
		unsigned long long temp = 1L << (ii - 1);
		PM[P[ii - 1] + 127] = PM[P[ii - 1] + 127] | temp;
	}
	////////////////////////////////////////////////////

	unsigned long long VN = 0x0000000000000000L;
	unsigned long long VP = 0xffffffffffffffffL;
	unsigned long long D0, HP, HN;

	long currDist = m;
	int minorCurr = k;
	int posiMinorCurr;

	for (int j = indiceStart; j < n; ++j)
	{

		posicaoAtual = j + 1;

		// Step(j); ///////////////////////////////////////////////
		D0 = (((PM[T[j] + 127] & VP) + VP) ^ VP) | PM[T[j] + 127] | VN;
		HP = VN | ~(D0 | VP);
		HN = VP & D0;
		VP = (HN << 1L) | ~(D0 | (HP << 1L));
		VN = (HP << 1L) & D0;
		///////////////////////////////////////////////////////////

		if ((HP & 0x8000000000000000L) == 0x8000000000000000L)
			currDist += 1;
		else if ((HN & 0x8000000000000000L) == 0x8000000000000000L)
			currDist -= 1;

		if (currDist <= k)
		{

			inicializador++;

			if (inicializador == 1)
			{
				++quantGrupo;
				minorCurr = currDist;
				posiMinorCurr = posicaoAtual;
			}
			else
			{

				if (currDist < minorCurr)
				{
					minorCurr = currDist;
					posiMinorCurr = posicaoAtual;
				}
				if (minorCurr == 0)
				{
					indexx.insert(indexx.begin(), minorCurr);
					indexx.insert(indexx.begin(), posiMinorCurr - m);
					break;
				}
			}
		}
		else
		{
			if (quantGrupo > 0)
			{
				indexx.insert(indexx.begin(), minorCurr);
				indexx.insert(indexx.begin(), posiMinorCurr - m);
				break;
			}
		}

		if (posicaoAtual == n)
		{
			if (quantGrupo > 0)
			{
				indexx.insert(indexx.begin(), minorCurr);
				indexx.insert(indexx.begin(), posiMinorCurr - m);
			}
		}
	}

	return indexx;
}

// Wu & Manber (1992)
// vector<int> searchShiftAnd(char *P, long m, char *T, long n)
// {

// 	int k = 0;

// 	vector<int> indexx;

// 	int inicializador = 0;
// 	int posicaoAnterior = 0;
// 	int quantGrupo = 0;
// 	int posicaoAtual = 0;
// 	int indiceMedio = 0;

// 	int MAXCHAR = 256;

// 	unsigned long long Masc[MAXCHAR], i, j, Ri, Rant, Rnovo;
// 	unsigned long long R[k];

// 	for (i = 0; i < MAXCHAR; i++)
// 		Masc[i] = 0L;

// 	for (i = 1; i <= m; i++)
// 	{
// 		Masc[P[i - 1] + 127] |= 1L << (m - i);
// 	}

// 	R[0] = 0L;
// 	Ri = 1 << (m - 1);
// 	for (j = 1; j <= k; j++)
// 		R[j] = (1L << (m - j)) | R[j - 1];

// 	for (i = 0; i < n; i++)
// 	{

// 		Rant = R[0];
// 		Rnovo = ((((unsigned long long)Rant) >> 1L) | Ri) & Masc[T[i] + 127];
// 		R[0] = Rnovo;

// 		// For Approximate Matching
// 		// for (j = 1; j <= k; j++)
// 		// {
// 		// 	Rnovo = ((((unsigned long long) R[j]) >> 1L) & Masc[T[i] + 127]) | Rant | (((unsigned long long)(Rant | Rnovo))  >> 1L);
// 		// 	Rant = R[j];
// 		// 	R[j] = Rnovo | Ri;
// 		// }

// 		posicaoAtual = i + 1;

// 		if ((Rnovo & 1L) != 0L)
// 		{

// 			indexx.insert(indexx.begin(), k);
// 			indexx.insert(indexx.begin(), (posicaoAtual - m));
// 			// cerr << "Posicao Atual: " << (posicaoAtual-m) << endl;
// 		}
// 	}

// 	return indexx;
// }
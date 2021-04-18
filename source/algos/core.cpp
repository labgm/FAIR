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
#include "readWeights.h"
#include <string.h>
#include <iostream>
#include <limits>
#include <stdlib.h>

using namespace std;

void readWeights(const int NUM_WEIGHTS, float* weights, fstream& lheFile)
{
    char lheFileLine[6];
    char lheFileChar;
    char lheFileWeight[14];
    bool foundPos = false;

    while (!foundPos)
    {
        lheFile.read ((char*)&lheFileLine,sizeof(lheFileLine));
        if (strcmp(lheFileLine,"<rwgt>")==0)
            foundPos = true;
        lheFile.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    for(int j = 0; j < NUM_WEIGHTS; ++j)
    {
        foundPos = false;
	while (!foundPos)
        {
	    lheFile.read ((char*)&lheFileChar,sizeof(lheFileChar));
            if (lheFileChar=='>')  foundPos = true;
        }
	lheFile.read ((char*)&lheFileChar,sizeof(lheFileChar));
        lheFile.read ((char*)&lheFileWeight,sizeof(lheFileWeight));
        weights[j] = atof(lheFileWeight);
        lheFile.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
}

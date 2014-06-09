#include "readWeights.h"
#include <string.h>
#include <iostream>
#include <limits>
#include <stdlib.h>
#include <string>

using namespace std;
/*
int main()
{
    std::vector<float> weights;
    fstream lheFile;
    lheFile.open("unweighted_events.lhe", ios::in | ios::binary); 

    for(int entry = 0; entry < 10; entry++)
    {
        weights.clear();
        readWeights(weights, lheFile);
        
        cout << "\nFor Entry " << entry << endl;

        for(unsigned int i = 0; i < weights.size(); i++)
        {
            cout << "weight " << i << " = " << weights[i] << endl;
        }
    }
    return 0;
}*/
void readWeights(vector<float>& weights, fstream& lheFile)
{
    char line[256];
    int position;   
    string lheFileLine;
    bool foundEventWeights = false;
    const int WEIGHT_SIZE = 14;

    weights.clear();

    do {
        lheFile.getline(line, 256);
        lheFileLine.assign(line);
    } while(lheFileLine.find("<rwgt>") ==  string::npos);

    if(lheFile.good() && !lheFile.eof())
        foundEventWeights = true;
    else
        cout << "Error in reading .lhe file\n";

    while(foundEventWeights)
    {
        lheFile.getline(line, 256);
        string lheFileLine = line;
        position = lheFileLine.find("</wgt>");
        if(position == string::npos)
            foundEventWeights = false;
        else
        {   
            const char* lheEventWeight =
                lheFileLine.substr(position - WEIGHT_SIZE - 1, position - 1).c_str();
            weights.push_back(atof(lheEventWeight));
        }
    }
}


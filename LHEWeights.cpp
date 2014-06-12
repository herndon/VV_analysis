#include "LHEWeights.h"
#include <iostream>

using namespace std;

LHEWeights::LHEWeights(const char* lheFileName)
{
    lheFile.open(lheFileName, std::ios::in | std::ios::binary);

    int position;   

    do {
        position = searchNextLine("<weightgroup");
    } while(position == string::npos && !lheFile.eof());
    
    if(position == string::npos)
        cout << "No weight info in .lhe file\n";
    else
    {
        while(searchNextLine("</weightgroup>") == string::npos)
        {
            weightNames.push_back(substrFromLine("param_card", "#"));
            lheFile.ignore(256, '\n');
        }
    }
}

const char* LHEWeights::substrFromLine(const string& identifier1, const string& identifier2)
{
    int startPos = lheFileLine.find(identifier1) + identifier1.size();
    int length = lheFileLine.find(identifier2) - startPos;

    return lheFileLine.substr(startPos, length).c_str();
}

int LHEWeights::searchNextLine(const string& searchSequence)
{
        lheFile.getline(line, 256);
        lheFileLine = line;
        return lheFileLine.find(searchSequence);
}

const std::vector<float>& LHEWeights::getVector()
{
    return weights;
}

const int LHEWeights::getNumWeights()
{
    return weightNames.size();
}
const std::vector<string>& LHEWeights::getNames()
{
    return weightNames;
}
void LHEWeights::readWeights()
{
    int position;   
    weights.clear();

    do {
        position = searchNextLine("<rwgt>");
    } while(position == string::npos && !lheFile.eof());

    if(lheFile.good() && !lheFile.eof())
    {
        while(searchNextLine("</rwgt>") == string::npos)
        {
            weights.push_back(atof(substrFromLine(">", "</wgt>")));
        }
    }
    else
        std::cout << "Error in reading .lhe file\n";
}

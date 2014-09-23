#include "LHEWeights.h"
#include <iostream>

using namespace std;

LHEWeights::LHEWeights(const char* lheFileName)
{
    lheFile.open(lheFileName, std::ios::in | std::ios::binary);

    size_t position;  
    SMWeightPos = 0;
    _unit_wgt = 1.118/34124.;

     do {
        position = searchNextLine("Unit wgt");
    } while(position == string::npos && !lheFile.eof());
    
    if(position == string::npos)
        cout << "Error in LHE file format. No unit weight found.\n";
    else
        _unit_wgt = atof(substrFromLine(": ", "\n"));

    do {
        position = searchNextLine("<weightgroup");
    } while(position == string::npos && !lheFile.eof());
    
    if(position == string::npos)
    {
        cout << "No weight info in LHE file. Using unit weight " 
             << _unit_wgt << " for all events. ";
        weightNames.push_back("unit wgt = " + std::to_string(_unit_wgt));
    }
    else
    {
        while(searchNextLine("</weightgroup>") == string::npos)
        {
            if(lheFileLine.find(" 0.0 ") != string::npos)
            {
                SMWeightPos = weightNames.size() - 1;
                weightNames.push_back("Standard Model");
            }
            else
                weightNames.push_back(substrFromLine("param_card", "#"));
            lheFile.ignore(256, '\n');
        }
    }
    formatNames();
}
void LHEWeights::formatNames()
{
    std::vector<std::pair<std::string, std::string>> paramNames;
    paramNames.push_back(std::make_pair("anoinputs 12", "f_{T1} ="));
    
    for(auto& weightName : weightNames)
    {    
        for(auto& paramName : paramNames)
        {
            if(weightName.find(paramName.first) != string::npos)
                weightName.replace(0, paramName.first.size() + 1, paramName.second);
        }
    }
}

const char* LHEWeights::substrFromLine(const string& identifier1, const string& identifier2)
{
    int startPos = lheFileLine.find(identifier1) + identifier1.size();
    int length = lheFileLine.find(identifier2) - startPos;

    return lheFileLine.substr(startPos, length).c_str();
}


const size_t LHEWeights::searchNextLine(const string& searchSequence)
{
        lheFile.getline(line, 256);
        lheFileLine = line;
        return lheFileLine.find(searchSequence);
}

const std::vector<float>& LHEWeights::getVector() const
{
    return weights;
}

const float LHEWeights::getSMWeight() const
{
    return weights[SMWeightPos];
}
const int LHEWeights::getSMWeightPos() const
{
    return SMWeightPos;
}
const int LHEWeights::getNumWeights() const
{
    return weightNames.size();
}
const std::vector<std::string>& LHEWeights::getNames() const
{
    return weightNames;
}
void LHEWeights::readWeights()
{
    size_t position;   
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
        weights.push_back(_unit_wgt);
}
LHEWeights::~LHEWeights()
{
    weights.clear();
    weightNames.clear();
}

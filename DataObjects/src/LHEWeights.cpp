#include "DataObjects/include/LHEWeights.h"
#include "Services/include/Exception.h"
#include <iostream>
#include <boost/algorithm/string.hpp>

using namespace std;

vvana::LHEWeights::LHEWeights(const char* lheFileName)
{
    lheFile_.open(lheFileName, std::ios::in | std::ios::binary);
    
    if (!lheFile_.is_open()) {
        throw vvana::Exception("LHEWeights::LHEWeights: LHE file not found");
        //has_weights_ = false;
        //unit_wgt_ = 1;
        //weightNames = {"Unweighted"};
    }
    else { 
        has_weights_ = findWeights();
        if (has_weights_)
            readWeightNames();
    }
}
bool vvana::LHEWeights::findWeights()
{
    unit_wgt_ = 1.;

    while (!searchNextLine("Unit wgt"))
    {    
        if (lheFile_.eof())
        {    
       throw vvana::Exception("LHEWeights::findWeights: Error in LHE file format. No unit weight found.");
        }
    }
    unit_wgt_ = atof(substrFromLine(": ", "\n"));
    
    while (!searchNextLine("<initrwgt"))
    {
        if (lheFileLine_.find("<init>") != std::string::npos)
        {
	  //out << "No weight info in LHE file. Using unit weight "
          //       << unit_wgt_ << " for all events.\n";
            weightNames.push_back("unit wgt = " + std::to_string(unit_wgt_));
            return false;
        }
    }
    return true;
}
void vvana::LHEWeights::readWeightNames()
{
    SMWeightPos = 0;
    while(!searchNextLine("</initrwgt>"))
    {    
        if (lheFileLine_.find("<weight id=") != string::npos)
        {
            bool foundSM = true;
            std::string weightName = "";
            int i = 0;
            do {
                std::string sbstr = substrFromLine("param_card ", " #");
                if (i++ > 0)
                    weightName += " ";
                weightName += sbstr;
                if (sbstr.find(" 0.0") == std::string::npos)
                    foundSM = false;

            } while (!searchNextLine("</weight>"));        
            if (foundSM)
            {
                weightName = "Standard Model";
                SMWeightPos = weightNames.size();
            }
            weightNames.push_back(weightName);
        }
    }
    formatNames();
}

void vvana::LHEWeights::formatNames()
{
    std::vector<std::pair<std::string, std::string>> paramNames;
    paramNames.push_back(std::make_pair("anoinputs 10", "f_{M7} ="));
    paramNames.push_back(std::make_pair("anoinputs 11", "f_{T0} ="));
    paramNames.push_back(std::make_pair("anoinputs 12", "f_{T1} ="));
    paramNames.push_back(std::make_pair("anoinputs 13", "f_{T2} ="));
    paramNames.push_back(std::make_pair("anoinputs 14", "f_{T3} ="));
    paramNames.push_back(std::make_pair("anoinputs 15", "f_{T4} ="));
    paramNames.push_back(std::make_pair("anoinputs 16", "f_{T5} ="));
    paramNames.push_back(std::make_pair("anoinputs 17", "f_{T6} ="));
    paramNames.push_back(std::make_pair("anoinputs 18", "f_{T7} ="));
    paramNames.push_back(std::make_pair("anoinputs 19", "f_{T8} ="));
    paramNames.push_back(std::make_pair("anoinputs 20", "f_{T9} ="));
    paramNames.push_back(std::make_pair("anoinputs 1", "f_{S0} ="));
    paramNames.push_back(std::make_pair("anoinputs 2", "f_{S1} ="));
    paramNames.push_back(std::make_pair("anoinputs 3", "f_{M0} ="));
    paramNames.push_back(std::make_pair("anoinputs 4", "f_{M1} ="));
    paramNames.push_back(std::make_pair("anoinputs 5", "f_{M2} ="));
    paramNames.push_back(std::make_pair("anoinputs 6", "f_{M3} ="));
    paramNames.push_back(std::make_pair("anoinputs 7", "f_{M4} ="));
    paramNames.push_back(std::make_pair("anoinputs 8", "f_{M5} ="));
    paramNames.push_back(std::make_pair("anoinputs 9", "f_{M6} ="));
    
    for(auto& weightName : weightNames)
    {   
        for(auto& paramName : paramNames)
        {
            size_t pos = weightName.find(paramName.first);
            if(pos != string::npos)
            {
                weightName.erase(pos, paramName.first.size());
                weightName.insert(pos, paramName.second);
            }
                
        }
    }
}

const char* vvana::LHEWeights::substrFromLine(const string& identifier1, const string& identifier2)
{
    int startPos = lheFileLine_.find(identifier1) + identifier1.size();
    int length = lheFileLine_.find(identifier2) - startPos;
    std::string substring = lheFileLine_.substr(startPos, length);
    boost::algorithm::trim(substring);
    return substring.c_str();
}


bool vvana::LHEWeights::searchNextLine(const string& searchSequence)
{
    if (lheFile_.good() && !lheFile_.eof()) 
    {
        getline(lheFile_, lheFileLine_);
        return lheFileLine_.find(searchSequence) != std::string::npos;
    }
    else if (!lheFile_.good()) {
      std::string error = "LHEWeights::searchNextLine: Problem reading weights from file" + lheFileLine_;
      throw vvana::Exception(error);
    }
    else {
      throw vvana::Exception("LHEWeights::searchNextLine: Reached end of file unexpectedly while parsing weights.");
    }
    return false;
}

const std::vector<float>& vvana::LHEWeights::getVector() const
{
    return weights;
}

const float vvana::LHEWeights::getSMWeight() const
{
    return weights[SMWeightPos];
}
const int vvana::LHEWeights::getSMWeightPos() const
{
    return SMWeightPos;
}
const int vvana::LHEWeights::getNumWeights() const
{
    return weightNames.size();
}
const std::vector<std::string>& vvana::LHEWeights::getNames() const
{
    return weightNames;
}
void vvana::LHEWeights::readWeights()
{
    weights.clear();

    if (has_weights_)
    {
        while (!searchNextLine("</rwgt>"))
        {
            if (lheFile_.eof()) {
	      //for (const auto & weight : weights)
		  //out << "Weight is : " << weight;
            }
            if (lheFileLine_.find("wgt id") != std::string::npos)
            {
                weights.push_back(atof(substrFromLine(">", "</wgt>")));
            }
        }
    }
    else
        weights.push_back(unit_wgt_);
}
void vvana::LHEWeights::setWeights(std::vector<float> newWeights)
{

  weights.clear();
  weights = newWeights;

}

vvana::LHEWeights::~LHEWeights()
{
    weights.clear();
    weightNames.clear();
}

#ifndef __LHEWEIGHTS_H__
#define __LHEWEIGHTS_H__

#include <string>
#include <vector>
#include <fstream>

class LHEWeights
{
private:
    std::fstream lheFile;
    std::vector<float> weights;
    unsigned int SMWeightPos;
    char line[256];
    std::string lheFileLine;
    std::vector<std::string> weightNames;
    void formatNames();
    const char* substrFromLine(const std::string&, const std::string&);
    const size_t searchNextLine(const std::string& searchSequence);
public:
    LHEWeights(const char* lheFileName);
    void readWeights();
    const std::vector<float>& getVector() const;
    const std::vector<std::string>& getNames() const; 
    const float getSMWeight() const;
    const int getNumWeights() const;
    const int getSMWeightPos() const;
};

#endif

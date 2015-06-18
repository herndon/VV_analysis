#ifndef __LHEWEIGHTS_H__
#define __LHEWEIGHTS_H__

#include <string>
#include <vector>
#include <fstream>

class LHEWeights
{
private:
    std::fstream lheFile_;
    float unit_wgt_;
    std::vector<float> weights;
    bool has_weights_;
    unsigned int SMWeightPos;
    std::string lheFileLine_;
    std::vector<std::string> weightNames;
    void formatNames();
    const char* substrFromLine(const std::string&, const std::string&);
    bool searchNextLine(const std::string& searchSequence);
    bool findWeights();
    void readWeightNames();
public:
    LHEWeights(const char* lheFileName);
    ~LHEWeights();
    void readWeights();
    const std::vector<float>& getVector() const;
    const std::vector<std::string>& getNames() const; 
    const float getSMWeight() const;
    const int getNumWeights() const;
    const int getSMWeightPos() const;
};

#endif

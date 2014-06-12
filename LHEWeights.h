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
    char line[256];
    std::string lheFileLine;
    std::vector<std::string> weightNames;
    const char* substrFromLine(const std::string&, const std::string&);
    int searchNextLine(const std::string& searchSequence);
public:
    LHEWeights(const char* lheFileName);
    void readWeights();
    const std::vector<float>& getVector();
    const std::vector<std::string>& getNames();
    const int getNumWeights();
};
#endif

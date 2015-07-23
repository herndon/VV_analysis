#ifndef __WZPLOTS_H__
#define __WZPLOTS_H__

#include <string>
#include "TH1.h"
#include "ExRootAnalysis/include/ExRootResult.h"
#include <vector>
#include "TObject.h"
#include "TFile.h"
#include "TH2.h"

namespace vvana {

class WZPlots
{
private:
    TFile* rootFile_;
    std::string rootFileName_;
    std::vector<std::map<std::string, std::map<std::string, TObject*>>> plotSets_;
    std::vector<std::string> weightNames_; 
public:
    WZPlots(const std::string& rootFileName, const std::string& baseFolder,
                     std::vector<std::string> weightNames);
    ~WZPlots();
    void addHist(const std::string& directory, const std::string& subdirectory,
                 const std::string& title, const std::string& xAxisTitle, 
                 const std::string& yAxisTitle, const int numBins, 
                 const float xMin, const float xMax);
    void add2DHist(const std::string& directory, const std::string& subdirectory,
                 const std::string& title, const std::string& xAxisTitle, 
                 const std::string& yAxisTitle, const int numBinsX, 
                 const float xMin, const float xMax, const int numBinsY,
                 const float yMin, const float yMax);
    void fillHist(const std::string& plotGroup, const std::string plot,
                  std::vector<float>& value, const std::vector<float>& weights,
                  const float luminosity);               
    void writeToFile();
};
} // end namespace vvana 

#endif // __WZPLOTS_H__ 

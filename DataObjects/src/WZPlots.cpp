#include "DataObjects/include/WZPlots.h"
#include "Services/include/Exception.h"
#include <string>

vvana::WZPlots::WZPlots(const std::string& rootFileName, const std::string& baseFolder,
                 std::vector<std::string> weightNames)                 
{
    rootFile_ = new TFile(rootFileName.c_str(), "RECREATE");
    rootFileName_ = rootFileName;
    if(baseFolder != "")
        rootFile_->mkdir(baseFolder.c_str());
    weightNames_ = weightNames;
        
    for(const auto& weightName : weightNames_)
    {
      if (rootFile_->mkdir(weightName.c_str()) == NULL)
      throw vvana::Exception("WZPlots::WZPlots: Error making weight directory, weightName is NULL");
    }
    plotSets_.resize(weightNames_.size());


}
void vvana::WZPlots::addHist(const std::string& directory, const std::string& subdirectory,
                      const std::string& title, const std::string& xAxisTitle, 
                      const std::string& yAxisTitle, const int numBins, 
                      const float xMin, const float xMax)
{
    int i = 0;
    for (auto& weightName : weightNames_)
    {
        std::string path = weightName + "/" + directory;
        if (!rootFile_->GetDirectory(path.c_str()))
            rootFile_->mkdir(path.c_str());
        rootFile_->cd(path.c_str());
        
        std::string weightTitle = title + " (" + weightName + ")";
        TH1F* hist = new TH1F(subdirectory.c_str(), weightTitle.c_str(),
                              numBins, xMin, xMax);
        hist->GetXaxis()->SetTitle(xAxisTitle.c_str());
        hist->GetYaxis()->SetTitle(yAxisTitle.c_str());
        plotSets_[i][directory][subdirectory] = hist;
        i++;
    } 
}
void vvana::WZPlots::add2DHist(const std::string& directory, const std::string& subdirectory,
                        const std::string& title, const std::string& xAxisTitle, 
                        const std::string& yAxisTitle, const int numBinsX, 
                        const float xMin, const float xMax, const int numBinsY,
                        const float yMin, const float yMax) 
{
    int i = 0;
    for (auto& weightName : weightNames_)
    {
        std::string path = weightName + "/" + directory;
        if (!rootFile_->GetDirectory(path.c_str()))
            rootFile_->mkdir(path.c_str());
        rootFile_->cd(path.c_str());
        
        std::string weightTitle = title + " (" + weightName + ")";
        TH2F* hist = new TH2F(subdirectory.c_str(), weightTitle.c_str(),
                              numBinsX, xMin, xMax, numBinsY, yMin, yMax);
        hist->GetXaxis()->SetTitle(xAxisTitle.c_str());
        hist->GetYaxis()->SetTitle(yAxisTitle.c_str());
        plotSets_[i][directory][subdirectory] = hist;
        i++;
    } 
}
void vvana::WZPlots::fillHist(const std::string& plotGroup, const std::string plot,
                       std::vector<float>& values, const std::vector<float>& weights,
                       const float luminosity)               
{
    if (weights.size() != plotSets_.size())
    {
      std::string error = "WZPlots::fillHist: Error, too many weights in histogram fill function, weights.size() = "
      + std::to_string(weights.size()) + " plotSets_.size() = "  + std::to_string(plotSets_.size());
      throw vvana::Exception(error);
    }

  for (unsigned int i = 0; i < weights.size(); i++)
    {
        if (values.size() == 1)
        {
            TH1F* hist = static_cast<TH1F*>(plotSets_[i][plotGroup][plot]);
            hist->Fill(values[0], weights[i]*luminosity);
        }
        else if (values.size() == 2)
        {
            TH2F* hist = static_cast<TH2F*>(plotSets_[i][plotGroup][plot]);
            hist->Fill(values[0], values[1], weights[i]*luminosity);
        }
    }
}
void vvana::WZPlots::writeToFile()
{
    int i = 0;
    for (auto& plotSet : plotSets_)
    {
        std::string weightPath = rootFileName_ + ":" + weightNames_[i++];
        for (auto& plotGroup : plotSet)
        {
            std::string path = weightPath + "/" + plotGroup.first;
            rootFile_->cd(path.c_str());
            for (auto& plot : plotGroup.second)
            {
                plot.second->Write();
            }
        }
    }
}
vvana::WZPlots::~WZPlots()
{
    for (auto& plotSet : plotSets_)
    {
        for (auto& particlePlots : plotSet) 
        {
            for (auto& plot : particlePlots.second)
                delete plot.second;
        }
    }
    delete rootFile_;
}

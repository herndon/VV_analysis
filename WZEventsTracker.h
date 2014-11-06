#ifndef __WZEVENTSTRACKER_H_
#define __WZEVENTSTRACKER_H__

#include "WZEvent.h"
#include "WZPlots.h"

struct WZTieredCuts
{
    unsigned int numHighPtLeptons;
    unsigned int numHighPtJets;
    float met;
    float ZMassRange;
};
struct WZKinematicCuts
{
    float WZTMass;
    float WZMass;
    float leptonMass;
    float diJetMass;
    float jetDeltaEta;
};
class WZEventsTracker
{
private:
    static constexpr float ON_SHELL_ZMASS = 91.1876f;
    WZEvent* wzEvent;
    WZPlots* plots;
    std::map<std::string, std::string> plotCuts;
    std::map<std::pair<std::string, std::string>, std::vector<float>> plotKeys;
    std::string eventType;
    std::map<std::string, int> eventCounters;
    bool useWeights;
    std::vector<float> crossSections;
    float luminosity;
    //float eventWeight;
    WZTieredCuts tieredCuts;
    WZKinematicCuts kinCuts;
    void assignValueToPlotKey(const std::string& keyPair1, 
                              const std::string& keyPair2, float value);
    void assignValuesToPlotKey(const std::string& keyPair1, 
                               const std::string& keyPair2, 
                               std::vector<float>& values);
    void assignValueToPlotKey(const std::string& keyPair1, 
                              const std::string& keyPair2);
    void assignValuesToPlotKey(const std::string& keyPair1, 
                               const std::string& keyPair2);
    void addHist(const std::string& plotGroup, const std::string& plot,
                 const std::string& title, const std::string& xtitle, 
                 const std::string& ytitle, const int numBins, const float xMin, 
                 const float xMax);
    void add2DHist(const std::string& plotGroup, const std::string& plot,
                   const std::string& title, const std::string& xtitle, 
                   const std::string& ytitle, const int numBinsX, const float xMin, 
                   const float xMax, const int numBinsY, const float yMin,
                   const float yMax);
    void processByLeptonType();
    bool passedKinematicCuts(std::string& cuts);
    void getLeptonPlotData(const std::string& cuts);
    void getWZTMassMjjPlotData(const std::string& cuts);
    void getJetPlotData(const std::string& cuts);
    void getWZPlotData(const std::string& cuts);
    void get4lPlotData(const std::string& cuts);
    void getDijetPlotData(const std::string& cuts);
    void getZPlotData(const std::string& cuts);
    void getMETPlotData(const std::string& cuts);
    void getDileptonPlotData(const std::string& cuts); 
    static bool sortParticlesByPt(const ParticleVector&, const ParticleVector&);
    static bool sortDileptonsByZMass(const TLorentzVector&, const TLorentzVector&);
public:
    WZEventsTracker(WZEvent* event, const std::string rootFileName,
                    const float luminosity);
    ~WZEventsTracker();
    void initializeWZTMassMjjPlot();
    void initializeDijetPlots();
    void initializeLeptonPlots();
    void initializeJetPlots();
    void initializeDileptonPlots();
    void initializeWZPlots();
    void initialize4lPlots();
    void initializeZPlots();
    void initializeMETPlots();
    void setLuminosity(float luminosity);
    void setLeptonSelection(int numLeptons);
    void setJetSelection(int numJets);
    void setMetCut(float metCut);
    void setJetMassCut(float diJetMass);
    void setWZTMassCut(float WZTMass);
    void setWZMassCut(float WZMass);
    void set4lMassCut(float leptonMass);
    void setZMassCut(float ZMass);
    void setEtajjCut(float eta_jj);
    void processEvent();
    void printEventInfo();
    void fillPlots();
    void writePlotsToFile(); 
};

#endif

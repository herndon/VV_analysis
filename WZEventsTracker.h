#include "WZEvent.h"
#include "WZPlots.h"

struct EventCounters
{
    int passedLeptonCut;
    int events3mu;
    int events2mu1e;
    int events2e1mu;
    int events3e;
    int passedJetCut;
    int passedMETCut;
    int passedZMassCut;
    //int passedM_jjSelection;
    //int passedEta_jjSelection;
};
struct WZTieredCuts
{
    int numHighPtLeptons;
    int numHighPtJets;
    float met;
    float ZMassRange;
};
struct WZKinematicCuts
{
    float WZTMass;
    float diJetMass;
    float jetDeltaEta;
};
class WZEventsTracker
{
private:
    const float ON_SHELL_ZMASS = 91.1876;
    WZEvent* wzEvent;
    WZPlots* plots;
    std::string nameBase;
    bool useWeights;
    std::vector<float> crossSections;
    WZTieredCuts tieredCuts;
    WZKinematicCuts kinCuts;
    EventCounters eventCounts;
    void processByLeptonType();
    bool passedKinematicCuts();
public:
    WZEventsTracker(ExRootResult* result, const int NUM_WEIGHTS, std::string name);
    ~WZEventsTracker();
    void setLeptonSelection(int numLeptons);
    void setJetSelection(int numJets);
    void setMetCut(float metCut);
    void setJetMassCut(float diJetMass);
    void setWZTMassCut(float WZTMass);
    void setZMassCut(float ZMass);
    void setEtajjCut(float eta_jj);
    void processEvent(WZEvent* wzEvent);
    void printEventInfo();
    void fillPlots();
};

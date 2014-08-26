#include "WZEventsTracker.h" 
#include <iostream>
#include <cmath>

using namespace std;

WZEventsTracker::WZEventsTracker(WZEvent* event, std::string rootFileName,
                                 const float luminosity)
{
    wzEvent = event;
    this->luminosity = luminosity;
    const LHEWeights* weights = wzEvent->getLHEWeights();
    std::vector<std::string> weightNames;
    if (weights == NULL)
    {
        useWeights = false;    
        weightNames.push_back("Unweighted");
    }
    else
    {
        weightNames = weights->getNames();
        useWeights = true;
        crossSections.resize(weights->getNumWeights());
        crossSections = {0.};
    }
    plots = new WZPlots(rootFileName, "", weightNames); 

    eventCounters["3e"] = 0;
    eventCounters["3mu"] = 0;
    eventCounters["3tau"] = 0;
    eventCounters["2t1e"] = 0;
    eventCounters["2t1mu"] = 0;
    eventCounters["2e1t"] = 0;
    eventCounters["2e1mu"] = 0;
    eventCounters["2mu1e"] = 0;
    eventCounters["2mu1t"] = 0;
    eventCounters["passedLeptonCut"] = 0;
    eventCounters["passedJetCut"] = 0;
    eventCounters["passedMETCut"] = 0;
    eventCounters["passedZMassCut"] = 0;


    kinCuts = {0.};
    tieredCuts.numHighPtLeptons = 3;
    tieredCuts.numHighPtJets = 2;
    tieredCuts.met = 0.;
    tieredCuts.ZMassRange = 1e10;

    initializeDijetPlots();
    initializeLeptonPlots();
    initializeJetPlots();
    initializeDileptonPlots();
    initializeWZPlots();
    initializeZPlots();
    initializeMETPlots();
}

WZEventsTracker::~WZEventsTracker()
{
    delete plots;
}
void WZEventsTracker::initializeDijetPlots()
{
    addPlot("dijet", "deltaEta","Dijet #Delta #eta", "#Delta#eta",
            "Events", 40, 0.0, 8.);
    addPlot("dijet", "mass", "Dijet Mass", "Mass (GeV/c^2)", 
            "Events", 40, -4.0, 4.0);
}
void WZEventsTracker::initializeLeptonPlots()
{
    for(unsigned int i = 1; i <= tieredCuts.numHighPtLeptons; i++)
    {
        addPlot("leptons", "lepton" + std::to_string(i) + "_pt",
                "Lepton" + std::to_string(i) + " p_{T}",
                "p_{T}", "Events", 150, 0.0, 2000.);
        addPlot("leptons", "lepton" + std::to_string(i) + "_eta", 
                "Lepton" + std::to_string(i) + " Eta", 
                "Eta", "Events", 40, -4.0, 4.0);
    }
}
void WZEventsTracker::initializeJetPlots()
{
    for(unsigned int i = 1; i <= tieredCuts.numHighPtJets; i++)
    {
        addPlot("jets", "jet" + std::to_string(i) + "_pt", 
                "Jet " +  std::to_string(i) +  " p_{T}", "p_{T}", "Events",
                 150, 0.0, 2000.);
        addPlot("jets", "jet" + std::to_string(i) + "_eta", 
                "Jet" + std::to_string(i) + " Eta", "Eta",  "Events", 40, -4.0, 4.0);
    }
}
void WZEventsTracker::initializeDileptonPlots()
{
    addPlot("DiffFlavDileptons","pt", "Dilepton p_{T} for Mixed Flavor Events ",
            "Dilepton p_{T} (GeV/c)", "Events", 150, 0.0, 100.0);
    addPlot("DiffFlavDileptons", "mass", "Dilepton Mass for Mixed Flavor Events",
           "Dilepton Mass (GeV/c^2)","Events", 150, 0.0, 200.0);

    addPlot("SameFlavDileptons", "nearZ_mass",
            "Dilepton Mass for Same Flavor Events (Near Z Mass)",
            "Dilepton p_{T} (GeV/c)","Events", 150, 0.0, 100.0);
    addPlot("SameFlavDileptons", "nearZ_pt",
            "Dilepton p_{T} for Same Flavor Events (Near Z Mass)",
            "Dilepton Mass (GeV/c^2)", "Events", 150, 0.0, 100.0);
    
    addPlot("SameFlavDileptons", "offZ_mass", 
            "Dilepton Mass for Same Flavor Events (Off Z Mass)",
            "Dilepton Mass (GeV/c^2)", "Events", 150, 0.0, 100.0);
    addPlot("SameFlavDileptons", "offZ_pt",
            "Dilepton p_{T} for Same Flavor Events (Off Z Mass)",
            "Dilepton Mass (GeV/c^2)", "Events", 150, 0.0, 100.0);
}
void WZEventsTracker::initializeWZPlots()
{
    addPlot("WZ", "transMass", "WZ Transverse Mass ", 
            "WZ Transverse Mass (GeV/c^2)", "Events", 200, 0.0, 2000.0);
    addPlot("WZ", "mass", "WZ Mass ", "WZ Mass (GeV/c^2)", "Events", 
            200, 0.0, 1000.0);
}
void WZEventsTracker::initializeZPlots()
{
    addPlot("Z", "pt", "Z p_{T}", "p_{T} (GeV/c)", "Events", 60, 0.0, 1000.0);
    addPlot("Z", "mass", "Z Mass", "Mass (GeV/c^2)", "Events", 60, 0.0, 150.0);
}
void WZEventsTracker::initializeMETPlots()
{
    addPlot("MET", "MET", "Missing E_{T} ", "Missing E_{T} (GeV/c^2)", "Events", 
            60, 0.0, 200.0);
}   
void WZEventsTracker::addPlot(const std::string& plotGroup, const std::string& plot,
                              const std::string& title, const std::string& xTitle, 
                              const std::string& yTitle, int numBins, 
                              float xMin, float xMax)
{
    std::string key1 = plotGroup;
    std::string key2 = plot;
    std::pair<std::string, std::string> key(key1, key2);
    plotKeys[key] = 0.;
    plots->addHist(key.first, key.second, title.c_str(), xTitle, yTitle, 
                   numBins, xMin, xMax);
}
void WZEventsTracker::getLeptonPlotData(const std::string& cuts)
{
    plotCuts["leptons"] = cuts;
    std::vector<ParticleVector> leptonVectors = wzEvent->getAllLeptons();
    
    std::sort(leptonVectors.begin(), leptonVectors.end(), sortParticlesByPt);
    if (leptonVectors.size() != tieredCuts.numHighPtLeptons)
    {
        cout << "Critical Error! Too many leptons!";
        exit(0);
    }

    for(unsigned int i = 0; i < tieredCuts.numHighPtLeptons; i++)
    {
        std::string leptonName = "lepton" + std::to_string(i+1);
        assignValueToPlotKey("leptons", leptonName + "_pt", leptonVectors[i].Pt());
        assignValueToPlotKey("leptons", leptonName + "_eta", leptonVectors[i].Eta());
    }
}

void WZEventsTracker::getJetPlotData(const std::string& cuts)
{
    plotCuts["jets"] = cuts;
    std::vector<ParticleVector> jetVectors = wzEvent->getAllJets();
   
    std::sort(jetVectors.begin(), jetVectors.end(), sortParticlesByPt);
    
    if (jetVectors.size() != tieredCuts.numHighPtJets)
    {
        cout << "Critical Error! Too many jets!";
        exit(0);
    }

    for(unsigned int i = 0; i < tieredCuts.numHighPtJets; i++)
    {
        assignValueToPlotKey("jets", "jet" + std::to_string(i+1) + "_pt", 
                             jetVectors[i].Pt());
        assignValueToPlotKey("jets", "jet" + std::to_string(i+1) + "_eta", 1.0);//jetVectors[i].Eta());
    }
}       
void WZEventsTracker::getWZPlotData(const std::string& cuts)
{
    plotCuts["WZ"] = cuts;
    assignValueToPlotKey("WZ","mass", wzEvent->getWZInvMass());
    assignValueToPlotKey("WZ","transMass", wzEvent->getWZTransMass());
}
void WZEventsTracker::getDijetPlotData(const std::string& cuts)
{
    plotCuts["diJets"] = cuts;
    assignValueToPlotKey("dijet","deltaEta", wzEvent->getJetDeltaEta());
    assignValueToPlotKey("dijet","deltaEta", wzEvent->getDiJetInvMass());
}
void WZEventsTracker::assignValueToPlotKey(const std::string& keyPair1, 
                                           const std::string& keyPair2,
                                           float value)
{
    std::pair<std::string, std::string> key(keyPair1, keyPair2);
    if (plotKeys.count(key))
    {
        plotKeys[key] = value;
    }
    else 
        std::cout << "\nNo " << keyPair1 << " " << keyPair2 << " Plot\n";
}
void WZEventsTracker::getZPlotData(const std::string& cuts)
{
    plotCuts["Z"] = cuts;
    assignValueToPlotKey("Z","pt", wzEvent->getZpt());
    assignValueToPlotKey("Z","mass", wzEvent->getZMass());
}
void WZEventsTracker::getMETPlotData(const std::string& cuts)
{
    plotCuts["MET"] = cuts;
    assignValueToPlotKey("MET","MET", wzEvent->getMET());
}
void WZEventsTracker::getDileptonPlotData(const std::string& cuts)
{
    std::vector<ParticleVector> leptonVectors = wzEvent->getAllLeptons();
    int dileptonType;

    if (eventType == "2e1mu" || eventType == "2mu1e" || eventType == "2e1t" 
        || eventType == "2mu1t" || eventType == "2t1e" || eventType == "2t1mu")
    {
        TLorentzVector Zdilepton;
        if(eventType == "2e1mu" || eventType == "2e1t")
            dileptonType = 11;
        else if(eventType == "2mu1e" || eventType == "2mu1t")
            dileptonType = 13;
        else
            dileptonType = 15;
        
        for(const auto& lepton : leptonVectors)
        {
            if(fabs(lepton.getType()) == dileptonType)
            {
                Zdilepton += (TLorentzVector&) lepton;
            }
        } 
        assignValueToPlotKey("DiffFlavDileptons","pt", Zdilepton.Pt());
        assignValueToPlotKey("DiffFlavDileptons","mass", Zdilepton.M());
    }
    else if (eventType == "3e" || eventType == "3mu" || eventType == "3tau")
    {
        std::vector<TLorentzVector> dileptonVects;
        for(unsigned int i = 0; i < leptonVectors.size(); i++)
        {
            for(unsigned int j = 0; j < leptonVectors.size() - 1; j++)
            { 
                if(i == j)
                    continue;
                if(leptonVectors[i].getType() + leptonVectors[j].getType() == 0)
                    dileptonVects.push_back(leptonVectors[i] + leptonVectors[j]);
            }
        }

        std::sort(dileptonVects.begin(), dileptonVects.end(), sortDileptonsByZMass);
        
        assignValueToPlotKey("SameFlavDileptons","nearZ_pt", dileptonVects[0].Pt());
        assignValueToPlotKey("SameFlavDileptons","nearZ_mass", dileptonVects[0].M());
        assignValueToPlotKey("SameFlavDileptons","offZ_pt", dileptonVects[1].Pt());
        assignValueToPlotKey("SameFlavDileptons","offZ_mass", dileptonVects[1].M());
    }
    else
    {
        std::cout << "\nAn error occured in the addDileptons() function"
                  << "of WZPlots class";
        exit(0);
    }
}
bool WZEventsTracker::sortParticlesByPt(const ParticleVector& particle1, 
                                const ParticleVector& particle2)
{
    return particle1.Pt() > particle2.Pt();
}

bool WZEventsTracker::sortDileptonsByZMass(const TLorentzVector& dilepton1, 
                                   const TLorentzVector& dilepton2)
{
    return fabs(dilepton1.M() - ON_SHELL_ZMASS) < 
                                    fabs(dilepton2.M() - ON_SHELL_ZMASS);
}

void WZEventsTracker::fillPlots()
{
    std::string plotGroup;
    std::string plot;

    for (auto& keyMap : plotKeys)
    {
        plotGroup = keyMap.first.first;
        plot = keyMap.first.second;
        plots->fillHist(plotGroup, plot, keyMap.second, 
                        wzEvent->getWeightsVector(), luminosity);
    }
}        
void WZEventsTracker::setLuminosity(float luminosity)
{
    this->luminosity = luminosity;
}

void WZEventsTracker::setLeptonSelection(int numLeptons)
{
    tieredCuts.numHighPtLeptons = numLeptons;
}
void WZEventsTracker::setJetSelection(int numJets)
{
    tieredCuts.numHighPtJets = numJets;
}
void WZEventsTracker::setMetCut(float metCut)
{
    tieredCuts.met = metCut;
}
void WZEventsTracker::setZMassCut(float ZMassRange)
{
    tieredCuts.ZMassRange = ZMassRange;
}
void WZEventsTracker::setWZTMassCut(float WZTMass)
{  
    kinCuts.WZTMass = WZTMass;
}
void WZEventsTracker::setJetMassCut(float diJetMass)
{
    kinCuts.diJetMass = diJetMass;
}
void WZEventsTracker::setEtajjCut(float eta_jj)
{
    kinCuts.jetDeltaEta = eta_jj;
}

//THESE ARE TIERED CUTS!
void WZEventsTracker::processEvent()
{
    //wzEvent = event;
    std::string cuts = "";

    if(wzEvent->getNumPostCutLeptons() == tieredCuts.numHighPtLeptons)
    {
        eventCounters["passedLeptonCut"]++;
        cuts += "Number of High p_T leptons = " 
                + std::to_string(tieredCuts.numHighPtLeptons) + "\n";
    }
    else
        return;
    
    if(wzEvent->getNumPostCutJets() == tieredCuts.numHighPtJets)
    {
        eventCounters["passedJetCut"]++;
        cuts += "Number of High p_T jets = " 
                + std::to_string(tieredCuts.numHighPtJets) + "\n";
    }
    else
        return;
    
    if(passedKinematicCuts(cuts))
    {
        processByLeptonType();
    }
    else
        return;
    
    if(wzEvent->getMET() > tieredCuts.met)
        eventCounters["passedMETCut"]++;
    else
        return;
    if(fabs(wzEvent->getZMass() - ON_SHELL_ZMASS) < tieredCuts.ZMassRange)     
        eventCounters["passedZMassCut"]++;
    else
        return;

    if(useWeights)
    {
        int i = 0;
        for(const auto& weight : wzEvent->getWeightsVector())
        {
            crossSections[i] += weight;
            i++;
        }
    }
    
    getLeptonPlotData(cuts);
    getJetPlotData(cuts);
    getWZPlotData(cuts);
    getDijetPlotData(cuts);
    getZPlotData(cuts);
    getMETPlotData(cuts);
    getDileptonPlotData(cuts); 
    
    fillPlots();
}

bool WZEventsTracker::passedKinematicCuts(std::string& cuts)
{
    if(wzEvent->getWZTransMass() < kinCuts.WZTMass)
        return false;
    else
        cuts += "WZ transverse mass > " + std::to_string(kinCuts.WZTMass) + "\n";
    
    if(fabs(wzEvent->getJetDeltaEta()) < kinCuts.jetDeltaEta)
        return false;
    else
        cuts +="Jet deltaEta > " + std::to_string(kinCuts.jetDeltaEta) + "\n";
    
    if(wzEvent->getDiJetInvMass() < kinCuts.diJetMass)
        return false;
    else
        cuts += "Dijet invariant mass > " + std::to_string(kinCuts.diJetMass)
                 + "\n";
    
    return true;
}

void WZEventsTracker::processByLeptonType()
{
    int numHighPtE = wzEvent->getNumPostCutElectrons();
    int numHighPtMu = wzEvent->getNumPostCutMuons();
    int numHighPtTau = wzEvent->getNumPostCutTaus();
    
    if(numHighPtE == 3 && numHighPtMu == 0 && numHighPtTau == 0)
    {
        eventCounters["3e"]++;
        eventType = "3e";
    }
    else if(numHighPtE == 2  && numHighPtMu == 1 && numHighPtTau == 0)
    {
        eventCounters["2e1mu"]++;
        eventType = "2e1mu";
    }
    else if(numHighPtE == 1 && numHighPtMu == 2 && numHighPtTau == 0)
    {
        eventCounters["2mu1e"]++;
        eventType = "2mu1e";
    }
    else if(numHighPtE == 0 && numHighPtMu == 3 && numHighPtTau == 0)
    {
        eventCounters["3mu"]++;
        eventType = "3mu";
    }
    else if(numHighPtE == 0 && numHighPtMu == 2 && numHighPtTau == 1)
    {
        eventCounters["2mu1t"]++;
        eventType = "2mu1t";
    }
    else if(numHighPtE == 0 && numHighPtMu == 1 && numHighPtTau == 2)
    {
        eventCounters["2t1mu"]++;
        eventType = "2t1mu";
    }
    else if(numHighPtE == 0 && numHighPtMu == 0 && numHighPtTau == 3)
    {
        eventCounters["3tau"]++;
        eventType = "3tau";
    }
    else if(numHighPtE == 1 && numHighPtMu == 0 && numHighPtTau == 2)
    {
        eventCounters["2t1e"]++;
        eventType = "2t1e";
    }
    else if(numHighPtE == 2 && numHighPtMu == 0 && numHighPtTau == 1)
    {
        eventCounters["2e1t"]++;
        eventType = "2e1t";
    }
    else
    {
        std::cout << "A Critical Error occurred in the processByLeptonType() "
                  << "function of WZEventsTracker Class.\n";
        std::cout << "num taus =" << numHighPtTau << std::endl;
        std::cout << "num es =" << numHighPtE << std::endl;
        std::cout << "num mus =" << numHighPtMu << std::endl;
        
        exit(0);
    }
}
void WZEventsTracker::printEventInfo()
{
    cout << "Kinematic cuts applied to all events: " << endl;
    cout << "di-Jet invariant mass > " << kinCuts.diJetMass << " GeV" << endl;
    cout << "Jet eta separation > " << kinCuts.jetDeltaEta << endl;
    cout << "WZ transverse mass > " << kinCuts.WZTMass << " GeV" << endl << endl;

    cout << "Number of events passing post cut lepton number = "
         << tieredCuts.numHighPtLeptons << ": "  << eventCounters["passedLeptonCut"]
         << endl;
    cout << "Number of 3 muon events: " << eventCounters["3mu"] << endl;
    cout << "Number of 1 electron 2 muon events:  " << eventCounters["2mu1e"] 
         << endl;
    cout <<  "Number of 2 electron 1 muon events:  " << eventCounters["2e1mu"]
         << endl;
    cout <<  "Number of 3 electon  events:  " << eventCounters["3e"] << endl;
    cout << "Number of 3 tau events: " << eventCounters["3tau"] << endl;
    cout << "Number of 1 tau 2 muon events:  " << eventCounters["2mu1t"] 
         << endl;
    cout <<  "Number of 2 electron 1 tau events:  " << eventCounters["2e1t"]
         << endl;
    cout <<  "Number of 1 electon 2 tau events:  " << eventCounters["2t1e"] << endl;
    cout <<  "Number of 1 muon 2 tau events:  " << eventCounters["2t1mu"] << endl;
    
    cout << "Number of events passing post cut jet number = "
         << tieredCuts.numHighPtJets << ": "  << eventCounters["passedJetCut"] 
         << endl;
    cout << "Number of events with MET > " << tieredCuts.met << " GeV: "
         << eventCounters["passedMETCut"] << endl;
    cout << "Number of events with Z Mass within " << tieredCuts.ZMassRange
         << " GeV of on-shell Z mass: " << eventCounters["passedZMassCut"] << endl;
    cout << "\n__________________________________________________________________\n";
}
void WZEventsTracker::writePlotsToFile()
{
    plots->writeToFile();
}

#include "WZPlots.h"
#include <iostream>

WZPlots::WZPlots(ExRootResult* result)
{
    WZPlots(result, "", 0, 0);
}
WZPlots::WZPlots(ExRootResult* result, std::string nameBase,
                            const int NUM_WEIGHTS, const int SMWeightPos)
{
    this->result = result;
    wztmassWeights.resize(NUM_WEIGHTS);
    std::string histObjName;

    for(int i = 1; i <= 3; i++)
    {
        histObjName = nameBase + "_lepton" + std::to_string(i) + "Pt";
        lepton_pt.push_back(result->AddHist1D(histObjName.c_str(), "Lepton P_{T}",
                                    "Lepton P_{T} (GeV/c)", "Number of Leptons",
                                     20, 0.0, 100.0));
    
        histObjName = nameBase + "_lepton" + std::to_string(i) + "Eta";
        lepton_eta.push_back(result->AddHist1D(histObjName.c_str(), "Lepton eta",
                                    "Lepton Eta", "Number of Leptons",
                                     20, -4.0, 4.0));
    }

    histObjName = nameBase + "_3eMassNearZ";
    diLeptonMass3eNearZ =  result->AddHist1D(histObjName.c_str(),
                 "Near Z Di-lepton Mass for 3e Events","di-lepton Mass (GeV/c^2)",
                 "Number of Events", 20, 0.0, 200.0);
 
    histObjName = nameBase + "_3ePtNearZ";
    diLeptonPt3eNearZ =  result->AddHist1D(histObjName.c_str(),
                 "Near Z Di-lepton Pt for 3e Events","di-lepton Pt (GeV/c)",
                 "Number of Events", 20, 0.0, 200.0);

    histObjName = nameBase + "_3eMassOffZ";
    diLeptonMass3eOffZ =  result->AddHist1D(histObjName.c_str(),
                 "Off Z Di-lepton Mass for 3e Events","di-lepton Mass (GeV/c^2)",
                 "Number of Events", 20, 0.0, 200.0);
 
    histObjName = nameBase + "_3ePtOffZ";
    diLeptonPt3eOffZ =  result->AddHist1D(histObjName.c_str(),
                 "Off Z Di-lepton Pt for 3e Events","di-lepton Pt (GeV/c)",
                 "Number of Events", 20, 0.0, 200.0);


    histObjName = nameBase + "_3muMassNearZ";
    diLeptonMass3muNearZ =  result->AddHist1D(histObjName.c_str(),
                 "Near Z Di-lepton Mass for 3mu Events","di-lepton Mass (GeV/c^2)",
                 "Number of Events", 20, 0.0, 200.0);
 
    histObjName = nameBase + "_3muPtNearZ";
    diLeptonPt3muNearZ =  result->AddHist1D(histObjName.c_str(),
                 "Near Z Di-lepton Pt for 3mu Events","di-lepton Pt (GeV/c)",
                 "Number of Events", 20, 0.0, 200.0);

    histObjName = nameBase + "_3muMassOffZ";
    diLeptonMass3muOffZ =  result->AddHist1D(histObjName.c_str(),
                 "Off Z Di-lepton Mass for 3mu Events","di-lepton Mass (GeV/c^2)",
                 "Number of Events", 20, 0.0, 200.0);
 
    histObjName = nameBase + "_3muPtOffZ";
    diLeptonPt3muOffZ =  result->AddHist1D(histObjName.c_str(),
                 "Off Z Di-lepton Pt for 3mu Events","di-lepton Pt (GeV/c)",
                 "Number of Events", 20, 0.0, 200.0);

////////////////////////////////////////////////////////////////////////////////

    histObjName = nameBase + "_2muMass";
    diLeptonMass2mu =  result->AddHist1D(histObjName.c_str(),
                 "Di-lepton Mass for 2mu Events","di-lepton Mass (GeV/c^2)",
                 "Number of Events", 20, 0.0, 200.0);
 
    histObjName = nameBase + "_2muPt";
    diLeptonPt2mu =  result->AddHist1D(histObjName.c_str(),
                 "Di-lepton Pt for 2mu Events","di-lepton Pt (GeV/c)",
                 "Number of Events", 20, 0.0, 200.0);

    histObjName = nameBase + "_2eMass";
    diLeptonMass2e =  result->AddHist1D(histObjName.c_str(),
                 "Di-lepton Mass for 2e Events","di-lepton Mass (GeV/c^2)",
                 "Number of Events", 20, 0.0, 200.0);
 
    histObjName = nameBase + "_2ePt";
    diLeptonPt2e =  result->AddHist1D(histObjName.c_str(),
                 "Di-lepton Pt for 2e Events","di-lepton Pt (GeV/c)",
                 "Number of Events", 20, 0.0, 200.0);


   



    histObjName = nameBase + "_jetPt";// + std::to_string(i) + "Pt";
    jet_pt = result->AddHist1D(histObjName.c_str(), "Jet P_{T}",
                                      "Jet P_{T} (GeV/c)", "Number of Jets",
                                       20, 0.0, 200.0);
    histObjName = nameBase + "_jetEta";// + std::to_string(i) + "Eta";
    jet_eta = result->AddHist1D(histObjName.c_str(), "Jet Eta",
                                      "Jet Eta", "Number of Jets",
                                       20, 0.0, 200.0);
    
    histObjName = nameBase + "_deltaEta_jj";
    deltaEta_jj = result->AddHist1D(histObjName.c_str(), "Delta Eta JJ",
                                    "Delta Eta JJ, GeV", "number of events",
                                     20, 0.0, 8.0);

    histObjName = nameBase + "_mjj";
    mjj = result->AddHist1D(histObjName.c_str(), "diJet Mass",
                                      "diJet Mass (GeV/c^{2})", "Number of Events",
                                       10, 0.0, 1000.0);

    histObjName = nameBase + "_Zpt";
    Z_pt = result->AddHist1D(histObjName.c_str(), "Z P_{T}",
                                      "Z p_{T} (GeV/c)", "Number of Events",
                                       20, 0.0, 1000.0);

    histObjName = nameBase + "_met";
    met = result->AddHist1D(histObjName.c_str(), "Missing E_{T}",
                            "Missing E_{T} (GeV)", "Number of Events",
                             20, 0.0, 200.0);

    histObjName = nameBase + "_wztmass";
    wztmass = result->AddHist1D(histObjName.c_str(), "WZ Transverse Mass",
                                "WZ Transverse Mass, GeV/c^{2})", "Number of Events",
                                 20, 0.0, 2000.0);

    histObjName = nameBase + "_wzmass";
    wzmass = result->AddHist1D(histObjName.c_str(), "WZ Mass",
                               "WZ Mass (GeV)", "Number of Events",
                                1000, 0.0, 10000.0);
    for(int i = 0; i < NUM_WEIGHTS; i++)
    {
        if(i == SMWeightPos)
            histObjName = nameBase + "_wztmass_" + "SM";
        else
            histObjName = nameBase + "_wztmass_weight" + std::to_string(i);
        
        wztmassWeights[i] = result->AddHist1D(histObjName.c_str(), "WZ Transverse Mass",
                                "WZ Transverse Mass, (GeV/c^{2})", "Number of Events",
                                 20, 0.0, 2000.0);
    }
}
void WZPlots::addLeptons(const std::vector<ParticleVector>& leptons, float scale)
{
    for(int i = 0; i < 3; i++)
    {
        lepton_pt[i]->Fill(leptons[i].Pt(), scale);
        lepton_eta[i]->Fill(leptons[i].Eta(), scale);
    }
}

void WZPlots::add2eDiLepton(const TLorentzVector& diLepton, float scale)
{
   diLeptonMass2e->Fill(diLepton.M(), scale);
   diLeptonPt2e->Fill(diLepton.Pt(), scale); 
}

void WZPlots::add2muDiLepton(const TLorentzVector& diLepton, float scale)
{
   diLeptonMass2mu->Fill(diLepton.M(), scale);
   diLeptonPt2mu->Fill(diLepton.Pt(), scale); 
}

void WZPlots::add1typeDiLeptons(const std::vector<TLorentzVector>& diLeptons, 
                                              std::string eventType, float scale)
{
    if(eventType == "3e")
    {
        diLeptonMass3eNearZ->Fill(diLeptons[0].M(), scale);
        diLeptonMass3eOffZ->Fill(diLeptons[1].M(), scale);

        diLeptonPt3eNearZ->Fill(diLeptons[0].Pt(), scale);
        diLeptonPt3eOffZ->Fill(diLeptons[1].Pt(), scale);
    }
    else if(eventType == "3mu")
    {
        diLeptonMass3muNearZ->Fill(diLeptons[0].M(), scale);
        diLeptonMass3muOffZ->Fill(diLeptons[1].M(), scale);

        diLeptonPt3muNearZ->Fill(diLeptons[0].Pt(), scale);
        diLeptonPt3muOffZ->Fill(diLeptons[1].Pt(), scale);
    }
    else
    {
        std::cout << "\nAn error occured in the add1typeDiLeptons() function.\n";
        exit(0);
    }
}

void WZPlots::addJet(float pt, float eta, float scale)
{
    jet_pt->Fill(pt, scale);
    jet_eta->Fill(eta, scale);
}
void WZPlots::fillMET(float met, float scale)
{
    this->met->Fill(met, scale);
}
void WZPlots::fillDeltaEta_jj(double deltaEta_jj, float scale)
{
    this->deltaEta_jj->Fill(deltaEta_jj, scale);
}
void WZPlots::fillMjj(double mjj, float scale)
{
    this->mjj->Fill(mjj, scale);
}
void WZPlots::fillZpt(double Zpt, float scale)
{
    this->Z_pt->Fill(Zpt, scale);
}
void WZPlots::fillWZTMass(double wztmass, float scale)
{
    this->wztmass->Fill(wztmass, scale);
}
void WZPlots::fillWZMass(double wzmass, float scale)
{
    this->wzmass->Fill(wzmass, scale);
}
void WZPlots::fillWZTMassWeights(float wztmass,const std::vector<float>& weights,
                                            float luminosity)
{
    for(unsigned int i = 0; i < wztmassWeights.size(); i++)
    {
        wztmassWeights[i]->Fill(wztmass, weights[i]*luminosity);
    }
}
void WZPlots::printHistograms(const char* type)
{
    result->Print(type);
}

#include "WZPlots.h"

WZPlots::WZPlots(ExRootResult* result)
{
    WZPlots(result, 1, "");
}
WZPlots::WZPlots(ExRootResult* result,const int NUM_WEIGHTS, std::string nameBase)
{
    this->result = result;
    wztmassWeights.resize(NUM_WEIGHTS);

    std::string histObjName = nameBase + "_electronPt";
    electron_pt = result->AddHist1D(histObjName.c_str(), "Electron P_{T}",
                                    "Electron P_{T} (GeV/c)", "Number of Electrons",
                                     20, 0.0, 100.0);
    
    histObjName = nameBase + "_muonPt";
    muon_pt = result->AddHist1D(histObjName.c_str(), "Muon P_{T}",
                               "muon P_{T} (GeV/c)", "Number of Muons",
                                20, 0.0, 100.0);

    histObjName = nameBase + "_electronEta";
    electron_eta = result->AddHist1D(histObjName.c_str(), "Electron eta",
                                    "Electron Eta", "Number of Electrons",
                                     20, -4.0, 4.0);

    histObjName = nameBase + "_muonEta";
    muon_eta = result->AddHist1D(histObjName.c_str(), "Muon Eta",
                                 "Muon Eta", "Number of Muons",
                                  20, -4.0, 4.0);
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
        histObjName = nameBase + "_wztmass_weight" + std::to_string(i);
        wztmassWeights[i] = result->AddHist1D(histObjName.c_str(), "WZ Transverse Mass",
                                "WZ Transverse Mass, GeV/c^{2})", "Number of Events",
                                 20, 0.0, 2000.0);
    }
}
void WZPlots::addElectron(float pt, float eta)
{
    electron_pt->Fill(pt);
    electron_eta->Fill(eta);
}

void WZPlots::addMuon(float pt, float eta)
{
    muon_pt->Fill(pt);
    muon_eta->Fill(eta);
}
void WZPlots::addJet(float pt, float eta)
{
    jet_pt->Fill(pt);
    jet_eta->Fill(eta);
}
void WZPlots::fillMET(float met)
{
    this->met->Fill(met);
}
void WZPlots::fillDeltaEta_jj(double deltaEta_jj)
{
    this->deltaEta_jj->Fill(deltaEta_jj);
}
void WZPlots::fillMjj(double mjj)
{
    this->mjj->Fill(mjj);
}
void WZPlots::fillZpt(double Zpt)
{
    this->Z_pt->Fill(Zpt);
}
void WZPlots::fillWZTMass(double wztmass)
{
    this->wztmass->Fill(wztmass);
}
void WZPlots::fillWZMass(double wzmass)
{
    this->wzmass->Fill(wzmass);
}
void WZPlots::fillWZTMassWeights(float wztmass,const std::vector<float>& weights)
{
    for(unsigned int i = 0; i < wztmassWeights.size(); i++)
     {
         wztmassWeights[i]->Fill(wztmass, weights[i]);
     }
}
void WZPlots::printHistograms(const char* type)
{
    result->Print(type);
}

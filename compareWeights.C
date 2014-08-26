#include <TFile.h>
#include "TCanvas.h"
#include "TH1.h"
#include "TROOT.h"
#include "TStyle.h"
#include "THStack.h"
#include "TLegend.h"
#include <string>
#include <iostream>

void compareWeights(const char* fileName, const std::string plotTitle,
                    const std::string plotType, const std::string plot,
                    const float xMin, const float xMax) {
    gStyle->SetOptStat(0);
    gROOT->ForceStyle();
 
    THStack* histStack = new THStack("histStack", plotTitle.c_str());
    TLegend* legend = new TLegend(.7 ,.75 ,.9 ,.9);
    
    std::vector<std::string> weightNames = {"f_T1 = 5e-12 ", "f_T1 = 1.5e-12 ", 
                                            "f_T1 = 1e-13 ", "Standard Model"};
    std::vector<int> colors = {kRed-7, kAzure-3, kGreen-6, kBlack, kBlue-4};

    TFile file(fileName);
    TCanvas* canvas = new TCanvas("canvas","Weight Comparison",150,10,990,660);
    canvas->cd();

    int numBins = 26;
    float rebinPoint = 0;
    double variableBins[numBins];
    for (int i = 0; i < numBins; i++)
    {
        if (i < 15)
            rebinPoint += 50.;
        else
            rebinPoint += 100.;
        variableBins[i] = rebinPoint;
    }

    for (unsigned int i = 0; i < weightNames.size(); i++)
    {
        std::string path = weightNames[i] + "/" + plotType + "/" + plot;
        TH1F* hist = static_cast<TH1F*>(file.GetObjectChecked(path.c_str(), "TH1F"));
        //hist->SetFillColor(colors[i]);
        hist->SetLineColor(colors[i]);
        hist->SetLineWidth(2);
        TH1* histRebin = hist->Rebin(25, "histRebin", variableBins);
        histStack->Add(histRebin, "hist E1");
        std::string legendName = weightNames[i] + " (GeV^{-4})";
        legend->AddEntry(hist, legendName.c_str());
    }
    histStack->Draw("nostack");
    histStack->GetXaxis()->SetTitle("WZ Transverse Mass");
    histStack->GetYaxis()->SetTitle("Events");
    histStack->GetXaxis()->SetRangeUser(xMin, xMax); 
    histStack->SetMaximum(50); 
    histStack->DrawClone("nostack");
    legend->DrawClone("same");
    canvas->Update();
    canvas->Print("WZTMassComp_ewk_zg_cuts.pdf");
}

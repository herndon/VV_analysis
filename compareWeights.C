#include <TFile.h>
#include "TCanvas.h"
#include "TH1.h"
#include "TROOT.h"
#include "TStyle.h"
#include "THStack.h"
#include "TLegend.h"
#include <string>
#include <iostream>

void addBackground(THStack* hist_stack, std::string& bg_fileName, 
                   const std::string& plotTitle, const std::string& plotType,
                   TLegend* legend, const char* legendEntry,
                   const double* variableBins, const int numBins);
 

void compareWeights(const char* fileName, const std::string plotTitle,
                    const std::string plotType, const std::string plot,
                    const std::string xlabel, const float xMin, const float xMax) {
    gStyle->SetOptStat(0);
    gROOT->ForceStyle();
 
    THStack* histStack = new THStack("histStack", plotTitle.c_str());
    TLegend* legend = new TLegend(.55 ,.55 ,.885 ,.875);
    legend->SetFillColor(kWhite);
     
    std::vector<std::string> weightNames = {"unit wgt = 0.000001"};
                                            //{"f_{T1} = 5e-12 ", "f_{T1} = 4e-12 ", 
                                            //"f_{T1} = 1.5e-12 ", "Standard Model"};
    std::vector<int> colors = {kRed-7, kAzure-3, kGreen-6, kBlack, kBlue-4};

    TFile signal_file(fileName);
    TCanvas* canvas = new TCanvas("canvas","Weight Comparison",150,10,990,660);
    canvas->cd();

    int numBins = 13;
    float rebinPoint = 0;
    double variableBins[numBins+1];
    for (int i = 0; i <= numBins; i++)
    {
        if (i < 10)
            rebinPoint += 75.;
        else if (i < 11)
            rebinPoint += 150.;
        else
            rebinPoint += 200;
        variableBins[i] = rebinPoint;
    }

    std::string bg_path = "/nfs_scratch/kdlong/wpz_zg_qcdbkgd/Events/run_01/";
    bg_path += fileName; 
    addBackground(histStack, bg_path, plotType, plot, legend, "WZ EWK + QCD",
                  variableBins, numBins);

    for (unsigned int i = 0; i < weightNames.size(); i++)
    {
        std::string path = weightNames[i] + "/" + plotType + "/" + plot;
        TH1F* hist = static_cast<TH1F*>(signal_file.GetObjectChecked(path.c_str(), 
                                                                     "TH1F"));
        //hist->SetFillColor(colors[i]);
        hist->SetLineColor(colors[i]);
        hist->SetLineWidth(2);
        TH1* histRebin = hist->Rebin(numBins, "histRebin", variableBins);
        histRebin->Sumw2();
        histStack->Add(histRebin, "hist E1");
        std::string legendName = "WZ EWK (" + weightNames[i] + ")";
        legend->AddEntry(hist, legendName.c_str(), "l");
    }
    
    histStack->Draw("nostack");
    histStack->GetXaxis()->SetTitle(xlabel.c_str());
    histStack->GetYaxis()->SetTitle("Events");
    //histStack->GetXaxis()->SetRangeUser(xMin, xMax); 
    //histStack->SetMaximum(125); 
    histStack->DrawClone("nostack");
    legend->DrawClone("same");
    canvas->Update();
    canvas->Print("WZTMassComp_zg_bg_cuts.pdf");
}
void addBackground(THStack* histStack, std::string& bg_fileName, 
                   const std::string& plotType, const std::string& plot,
                   TLegend* legend, const char* legendEntry,
                   const double* variableBins, const int numBins) {
    TFile* bg_file = new TFile(bg_fileName.c_str());
    std::string path = "unit wgt = 0.000001/" + plotType + "/" + plot;
    TH1F* bg_hist = static_cast<TH1F*>(bg_file->GetObjectChecked(path.c_str(),"TH1F"));
    bg_hist->SetFillColor(kCyan-4);
    bg_hist->SetFillStyle(3004);
    bg_hist->SetLineColor(kCyan+2);
    bg_hist->SetLineWidth(2);
    bg_hist->DrawClone();
    TH1* histRebin = bg_hist->Rebin(numBins, "histRebin", variableBins);
    
    histStack->Add(histRebin, "hist E1");
    legend->AddEntry(bg_hist, legendEntry, "lf");
}

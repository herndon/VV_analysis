#!/usr/bin/env python
import ROOT as rt
#import CMS_lumi, tdrstyle
#import array

def main():
    hist = getHistFromFile("generatorWeights.root", "f_{T1} = 7.5e-12",
                               "WZ", "mass")
    print 'here type is'
    print type(hist)
    
    makePlot(hist, "generatorWeights.root", "~/public_html/test/test.pdf", "this", 
                   "title", "x", "y", 0, 3000, 0, 5000)

def getHistFromFile(file_name, base_folder, plot_type, plot):
    path_to_hist = base_folder + "/" + plot_type + "/" + plot
    file = rt.TFile(file_name)
    hist = rt.TH1F(file.Get(path_to_hist))
    print 'In function type is'
    print type(hist)
    if not hist:
        print 'Problem getting hist from file'
        exit(0)
    return hist
def makePlot(hist, name, options, legendName, plot_title, xlabel, ylabel, 
             xmin, xmax, ymin, ymax):
    canvas = rt.TCanvas("canvas", "canvas", 800, 600)
    legend = rt.TLegend(.5 ,.65 ,.885 ,.875)
    
    legend.SetFillColor(rt.kWhite)
    legend.AddEntry(legendName)
    #gStyle.SetTitleY(0.99) 
    canvas.SetMargin(.12, .12, .12, .12)
    #hist.GetZaxis().SetTitle("#times 10^{6}", "h")
    #hist.SetTitleOffset(1.7, 1.,"z")
    #hist.GetYaxis().SetTitleOffset(-0.05)
    #hist.GetXaxis().SetNoExponent(false)
    hist = getHistFromFile("generatorWeights.root", "f_{T1} = 7.5e-12", "WZ", "mass")
    h =  rt.TH1F("h","h; m_{e^{+}e^{-}} (GeV); Events / 0.5 GeV",80,70,110)
    print 'type is now '
    print type(hist)
    h.Draw()
    hist.SetFillColor(rt.tOrange-8)
    hist.SetLineColor(rt.kRed+4)
    hist.SetLineWidth(2)
    hist.Rebin(4)
    hist.Draw(options)
    if (xmin < xmax):
        hist.GetXaxis().SetRangeUser(xmin, xmax)
    if (ymin < ymax):
        hist.GetYaxis().SetRangeUser(ymin, ymax)
    hist.GetXaxis().SetTitle("haha")#xlabel.c_str())
    hist.GetYaxis().SetTitle("hehe")#ylabel.c_str())
    print "\nNo here the title is %s" % plotTitle
    
    #canvas.SetLogy()
    canvas.Print(name.c_str()) 
    
if __name__ == "__main__":
    main() 

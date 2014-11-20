#!/usr/bin/env python
import ROOT as rt
import CMS_lumi, tdrstyle
import argparse

def main():
    plot_info = getSimplePlotInfo()
    canvas = rt.TCanvas("canvas", "canvas", 800, 600)
    hist = getHistFromFile(plot_info.path + "/" + plot_info.file_name, 
                           plot_info.folder, plot_info.plot_group, 
                           plot_info.plot_type)
    setHistAttributes(hist, plot_info.title, plot_info.xlabel,
                      plot_info.ylabel, plot_info.xmin, plot_info.xmax, 
                      plot_info.ymin, plot_info.ymax, plot_info.rebin) 
    if type(hist) == "<class '__main__.TH2F'>":
        hist_opts = "colz"
    else:
        hist_opts = "hist"
    makePlot(canvas, hist, hist_opts, plot_info.output_file)
def getHistFromFile(file_name, base_folder, plot_type, plot):
    path_to_hist = base_folder + "/" + plot_type + "/" + plot
    file = rt.TFile(file_name)
    hist = file.Get(path_to_hist)
    if not hist:
        print 'Failed to get %s from file %s' % (path_to_hist, file_name)
        exit(0)
    hist.SetDirectory(rt.gROOT) # detach "hist" from the file
    return hist
def setHistAttributes (hist, title, xlabel, ylabel, xmin, xmax, 
                       ymin, ymax, rebin):
    hist.SetFillColor(rt.kOrange-8)
    hist.SetLineColor(rt.kRed+4)
    hist.SetLineWidth(2)
    if rebin != 0:
        hist.Rebin(rebin)
    hist.Draw()
    if xmin < xmax:
        hist.GetXaxis().SetRangeUser(xmin, xmax)
    if ymin < ymax:
        hist.GetYaxis().SetRangeUser(ymin, ymax)
    hist.GetXaxis().SetTitle(xlabel)
    hist.GetYaxis().SetTitle(ylabel)
def makePlot (canvas, hist, hist_opts, output_file):
    #legend = rt.TLegend(.5 ,.65 ,.885 ,.875)
           #gStyle.SetTitleY(0.99) 
    #canvas.SetMargin(.12, .12, .12, .12)
    #hist.GetZaxis().SetTitle("#times 10^{6}", "h")
    #hist.SetTitleOffset(1.7, 1.,"z")
    #hist.GetYaxis().SetTitleOffset(-0.05)
    #hist.GetXaxis().SetNoExponent(false)
    #canvas.SetLogy()
    #draw the lumi text on the canvas
    hist.Draw(hist_opts)
    setTDRStyle(canvas, 100, 13, True) 
    canvas.cd()
    canvas.Update()
    canvas.RedrawAxis()
    #frame = canvas.GetFrame()
    #frame.Draw()
    #legend.SetFillColor(rt.kWhite)
    #legend.AddEntry(hist, legendName)

    #legend.Draw("same")
    canvas.Print(output_file) 
def setTDRStyle(canvas, luminosity, energy, printCMS):
    tdrstyle.setTDRStyle() 
    if printCMS:
        if energy == 13:
            CMS_lumi.lumi_13TeV = "100 fb^{-1} MC"
            iPos = 11
            CMS_lumi.CMS_lumi(canvas, 4, iPos)
        elif energy == 8:
            CMS_lumi.lumi_13TeV = "100 fb^{-1} MC"
        CMS_lumi.writeExtraText = 1
        CMS_lumi.extraText = "Preliminary"
def getBasicParser():
    parser = argparse.ArgumentParser()
    parser.add_argument('-n', '--file_name', type=str, required=True, 
                        help="Name of root file in which histogram is stored.")
    parser.add_argument('-o', '--output_file', type=str, required=True,
                        help="Name produced plot file (type pdf/png/jpg etc.).")
    parser.add_argument('-p', '--path', type=str, default='.', required=False,
                        help="Path to root file")
    parser.add_argument('-t', '--title', type=str, default="", required=False, 
                        help="Plot title")
    parser.add_argument('-f', '--folder', type=str, required=True, 
                        help="Folder (in root file) to histogram")
    parser.add_argument('-g', '--plot_group', type=str, required=True, 
                        help="Plot group (folder in root file)")  
    parser.add_argument('-y', '--plot_type', type=str, required=True, 
                        help="Plot type (hist name in root file)")  
    parser.add_argument('--xlabel', type=str, required=False, default="", 
                        help="x axis label")
    parser.add_argument('--ylabel', type=str, required=False, default="", 
                        help="y axis label")
    parser.add_argument('--xmin', type=float, required=False, default=0, 
                        help="minimum x value")
    parser.add_argument('--xmax', type=float, required=False, default=0, 
                        help="maximum x value")   
    parser.add_argument('--ymin', type=float, required=False, default=0, 
                        help="minimum y value")
    parser.add_argument('--ymax', type=float, required=False, default=0, 
                        help="maximum y value")
    parser.add_argument('--rebin', type=int, required=False, default=0, 
                        help="Simple 1D rebin (integer) value")
    return parser.parse_args()
    
if __name__ == "__main__":
    main()

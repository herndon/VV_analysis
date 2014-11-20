#!/usr/bin/env python
import WpZ_plots as plotter
import ROOT

def main():
    plot_info = getSignalArgs()
    
    signal_info = {"comb_file" : plot_info.pop("comb_file"), 
                   "bkgd_file" : plot_info.pop("bkgd_file"),
                   "bkgd_wgt" : plot_info.pop("bkgd_wgt"), 
                   "comb_wgt" : plot_info.pop("comb_wgt")} 
    plot_info["folder"] = ""
    signal_hist = getSignal(signal_info, plot_info)
    if "compare_file" in plot_info and "compare_wgt" in plot_info:
        plot_info["file_name"] = plot_info["compare_file"]
        plot_info["folder"] = plot_info["compare_wgt"]
        hist_stack = ROOT.THStack()
        hist_stack.Add(signal_hist)
        plotter.addHistToStack(hist_stack, plot_info, "hist", ROOT.kBlue-8, 0) 
        plotter.makePlot(hist_stack, "hist nostack", plot_info) 
    else:
        plotter.makePlot(signal_hist, "hist", plot_info) 
def getSignal(signal_info, plot_info):
    plot_info["folder"] = signal_info["bkgd_wgt"]
    plot_info["file_name"] = signal_info["bkgd_file"]
    bkgd_hist = plotter.getHistFromFile(plot_info)
    plotter.setHistAttributes (bkgd_hist, plot_info, ROOT.kOrange-8, 0)
    
    plot_info["folder"] = signal_info["comb_wgt"]
    plot_info["file_name"] = signal_info["comb_file"]
    combined_hist = plotter.getHistFromFile(plot_info)
    plotter.setHistAttributes (combined_hist, plot_info, ROOT.kOrange-8, 0)
    
    combined_hist.Add(bkgd_hist, -1.)
    return combined_hist
def getSignalArgs():
    parser = plotter.getBasicParser()
    parser.add_argument("--comb_file", required=True, help="Signal+bkgd") 
    parser.add_argument("--bkgd_file", required=True, help="Signal+bkgd") 
    parser.add_argument("--comb_wgt", required=True, help="comb_wgt")
    parser.add_argument("--bkgd_wgt", required=True, help="bkgd_wgt")
    parser.add_argument("--compare_file", required=False, help="bkgd_wgt")
    parser.add_argument("--compare_wgt", required=False, help="bkgd_wgt")
    #subparsers = parser.add_subparsers()
    #compare = subparsers.add_parser("compare", help="for weights")
    #compare.add_argument("--compare_file", required=True, help="compare file")

    return vars(parser.parse_args())

if __name__ == "__main__":
    main()

#!/usr/bin/env python
import WpZ_plots as plots
import ROOT as rt

def main():
    weight_plot_info = getWeightArgs()
    weights = {"f_{T1} = 7.5e-12", "f_{T1} = 1.5e-12", "f_{T1} = 5e-13"}
    plotSameCanvas = True

    hist_opts = "hist"
    hist_stack = rt.THStack()
    
    cols = [rt.kOrange-8, rt.kBlue-4, rt.kBlue+8]

    i = 0
    for weight in weights:
        weight_plot_info["folder"] = weight
        print cols[i]
        if plotSameCanvas: 
            plots.addHistToStack(hist_stack, weight_plot_info, hist_opts, 
                                 cols[i], 0)          
        else:
            hist = plots.getHistFromFile(weight_plot_info)
            plots.setHistAttributes(hist, weight_plot_info, rt.kYellow-i, 
                                    rt.kYellow+i)
            plot_info["output_file"] = "~/public_html/test/test%s.pdf" % i
            plots.makePlot(hist, hist_opts, plot_info) 
        i += 1
    if plotSameCanvas:
        plots.makePlot(hist_stack, "hist nostack", plot_info)
def getWeightArgs():
    parser = plots.getBasicParser()
    return vars(parser.parse_args())

if __name__ == "__main__":
    main()

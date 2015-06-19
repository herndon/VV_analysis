#include "TROOT.h"
#include "TSystem.h"
#include "THStack.h"
#include "TLegend.h"
#include "TPaveText.h"

#include "ExRootAnalysis/include/ExRootTreeReader.h"
#include "ExRootAnalysis/include/ExRootTreeWriter.h"
#include "ExRootAnalysis/include/ExRootTreeBranch.h"
#include "ExRootAnalysis/include/ExRootResult.h"
#include "ExRootAnalysis/include/ExRootUtilities.h"

#include "Services/include/Config.h"
#include "DataObjects/include/WZEvent.h"
#include "DataObjects/include/WZEventsTracker.h"
#include "DataObjects/include/WZEventList.h"
#include "Modules/include/AnalyseEvents.h"
#include <iostream>
#include <vector>

//------------------------------------------------------------------------------

int main( int argc, char *argv[])
{

// Configure VV_ana using general Config class
    std::ifstream configfile("configfile.txt");
    vvana::Config config(configfile);

    // iostream file for debug output
    std::ofstream debugfile(config.debugFileName().c_str());

    if (config.debugLevel() == 0 ) std::cout << "VV_ana running " << std::endl;

    if (config.debugLevel() >=1 ) config.printConfig(debugfile);


    std::vector<const char*> root_files;
    std::vector<const char*> lhe_files;
    
    root_files.push_back(config.rootFileName().c_str());
    lhe_files.push_back("NONE");

    std::vector<WZEventList> event_lists;
    for (unsigned int i = 0; i < root_files.size(); i++) {
        WZEventList event_list(root_files[i], lhe_files[i]);
        event_lists.push_back(event_list);
    }
 
   AnalyseEventsModule analyseEventsModule(config.debugLevel());
    analyseEventsModule.AnalyseEvents(event_lists);
    
    return 0;   
}
//------------------------------------------------------------------------------

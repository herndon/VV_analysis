#ifndef __WZEventList_h__
#define __WZEventList_h__

#include "ExRootAnalysis/include/ExRootTreeReader.h"
#include "ExRootAnalysis/include/ExRootUtilities.h"
#include "TClonesArray.h"
#include "ExRootAnalysis/include/ExRootClasses.h"
#include "DataObjects/include/WZEvent.h"

namespace vvana {

class WZEventList
{
private: 
    TChain* chain;
    ExRootTreeReader* tree_reader;
    WZEvent* wzEvent;
    unsigned int num_entries;    
public:
    TClonesArray* branch_gen_particle;
    TClonesArray* branchWeights;
    WZEventList(const char* lhe_file_name, const char* root_file_name);
    void setLeptonCuts(const float min_put_cut, const float max_eta_cut);
    void setJetCuts(const float min_put_cut, const float max_eta_cut);
    unsigned int getNumEntries();
    const std::vector<std::string>& getWeightNames();
    WZEvent* getEvent(unsigned int entry);
};
} // end namespacei vvana
#endif //__WZEventList_h__

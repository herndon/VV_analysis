#include "DataObjects/include/WZEventList.h"
#include "ExRootAnalysis/include/ExRootClasses.h"
#include <iostream>

vvana::WZEventList::WZEventList(const char* root_file_name, const char* lhe_file_name) {
    chain = new TChain("LHEF");
    chain->Add(root_file_name);
    tree_reader = new ExRootTreeReader(chain);
    num_entries = tree_reader->GetEntries();
    branchWeights = tree_reader->UseBranch("Rwgt");
    branch_gen_particle = tree_reader->UseBranch("Particle");
    wzEvent = new WZEvent(lhe_file_name);
}
void vvana::WZEventList::setLeptonCuts(float min_pt_cut, float max_eta_cut) {
    wzEvent->setLeptonCuts(min_pt_cut, max_eta_cut);
}
void vvana::WZEventList::setJetCuts(float min_pt_cut, float max_eta_cut) {
    wzEvent->setJetCuts(min_pt_cut, max_eta_cut);
}
const std::vector<std::string>& vvana::WZEventList::getWeightNames() {
    return wzEvent->getWeightNames();
}
unsigned int vvana::WZEventList::getNumEntries() {
    return num_entries;
}
vvana::WZEvent* vvana::WZEventList::getEvent(unsigned int entry) {
    wzEvent->resetEvent();
    tree_reader->ReadEntry(entry);
    wzEvent->loadEvent(branch_gen_particle,branchWeights);

    return wzEvent;
}

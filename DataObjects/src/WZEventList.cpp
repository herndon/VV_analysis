#include "DataObjects/include/WZEventList.h"

WZEventList::WZEventList(const char* root_file_name, const char* lhe_file_name) {
    chain = new TChain("LHEF");
    chain->Add(root_file_name);
    tree_reader = new ExRootTreeReader(chain);
    
    num_entries = tree_reader->GetEntries();
    branch_gen_particle = tree_reader->UseBranch("Particle");
    wzEvent = new WZEvent(lhe_file_name);
}
void WZEventList::setLeptonCuts(float min_pt_cut, float max_eta_cut) {
    wzEvent->setLeptonCuts(min_pt_cut, max_eta_cut);
}
void WZEventList::setJetCuts(float min_pt_cut, float max_eta_cut) {
    wzEvent->setJetCuts(min_pt_cut, max_eta_cut);
}
const std::vector<std::string>& WZEventList::getWeightNames() {
    return wzEvent->getWeightNames();
}
unsigned int WZEventList::getNumEntries() {
    return num_entries;
}
WZEvent* WZEventList::getEvent(unsigned int entry) {
    wzEvent->resetEvent();
    tree_reader->ReadEntry(entry);
    wzEvent->loadEvent(branch_gen_particle);
    return wzEvent;
}

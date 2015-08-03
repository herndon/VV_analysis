#include "DataObjects/include/Particle.h"
#include "DataObjects/include/VVEvent.h"
#include "Analysis/include/WZAnalysis.h"


vvana::WZAnalysis::WZAnalysis(const VVEvent& vvEvent):
  _wzEvent(false),_vvEvent(vvEvent)
{
  _wzEvent = false;

}

vvana::WZAnalysis::WZAnalysis(bool wzEvent,const VVEvent & vvEvent, int w, int z, const std::vector<int> & zLeptons, const std::vector<int> & wLeptons, int wNeutrino, int wLepton, const std::vector<int> & jets):
  _wzEvent(wzEvent),_vvEvent(vvEvent),
   _w(w),_z(z),_zLeptons(zLeptons),_wLeptons(wLeptons),_wNeutrino(wNeutrino),_wLepton(wLepton),_jets(jets)
{
}

 

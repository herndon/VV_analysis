#include "DataObjects/include/Particle.h"
#include "DataObjects/include/VVEvent.h"
#include "Analysis/include/WZAnalysis.h"
#include "Analysis/include/VVAnalysisFunctions.h"
#include "Analysis/include/WZAnalysisFunctions.h"


const vvana::WZAnalysis vvana::BuildWZAnalysis(const VVEvent& vvEvent){

  bool wzEvent = false;

  // All particles in VVEvent referenced by index
  std::vector<int> ws = wlnus(vvEvent);
  std::vector<int> zs = zlls(vvEvent);
  // Vectors are pT ordered
  std::vector<int> wLeptons;
  int wNeutrino = -1;
  int wLepton = -1;
  std::vector<int> zLeptons;
  //std::vector<int> jets = jets(vvEvent);

  if (ws.size()==1&&zs.size()==1) {
    wzEvent = true;
    if (vvEvent.particles()[vvEvent.particles()[zs[0]].daughters()[0]].lorentzVector().Pt()>=
	vvEvent.particles()[vvEvent.particles()[zs[0]].daughters()[1]].lorentzVector().Pt()){

      zLeptons.push_back(vvEvent.particles()[zs[0]].daughters()[0]);
      zLeptons.push_back(vvEvent.particles()[zs[0]].daughters()[1]);
      
    } else {

      zLeptons.push_back(vvEvent.particles()[zs[0]].daughters()[1]);
      zLeptons.push_back(vvEvent.particles()[zs[0]].daughters()[0]);

    }
     if (vvEvent.particles()[vvEvent.particles()[ws[0]].daughters()[0]].lorentzVector().Pt()>=
	vvEvent.particles()[vvEvent.particles()[ws[0]].daughters()[1]].lorentzVector().Pt()){

      wLeptons.push_back(vvEvent.particles()[ws[0]].daughters()[0]);
      wLeptons.push_back(vvEvent.particles()[ws[0]].daughters()[1]);
      
    } else {

      wLeptons.push_back(vvEvent.particles()[ws[0]].daughters()[1]);
      wLeptons.push_back(vvEvent.particles()[ws[0]].daughters()[0]);

    }
     wNeutrino = neutrinos(vvEvent)[0];    
     if (std::abs(vvEvent.particles()[vvEvent.particles()[ws[0]].daughters()[0]].type())==11||
	 std::abs(vvEvent.particles()[vvEvent.particles()[ws[0]].daughters()[0]].type())==11 ||
	 std::abs(vvEvent.particles()[vvEvent.particles()[ws[0]].daughters()[0]].type())==11) {
       wLepton = vvEvent.particles()[ws[0]].daughters()[0]; 
     } else {
       wLepton = vvEvent.particles()[ws[0]].daughters()[1]; 
     }
  }
  
  return WZAnalysis(wzEvent,vvEvent,ws[0],zs[0],zLeptons,wLeptons,wNeutrino,wLepton,jets(vvEvent));
  
}

  

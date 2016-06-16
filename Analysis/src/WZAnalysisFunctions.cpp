#include "DataObjects/include/Particle.h"
#include "DataObjects/include/VVEvent.h"
#include "Analysis/include/WZAnalysis.h"
#include "Analysis/include/VVAnalysisFunctions.h"
#include "Analysis/include/WZAnalysisFunctions.h"
#include <iostream>

const vvana::WZAnalysis vvana::BuildWZAnalysis(const VVEvent& vvEvent){

  bool wzEvent = false;
  std::cout << "In BuildWZAnalysis" << std::endl;

  // All particles in VVEvent referenced by index
  std::vector<int> ws = wlnus(vvEvent);
  std::cout << "Found w" << std::endl;
  std::vector<int> zs = zlls(vvEvent);
  std::cout << "Found z" << std::endl;
  // Vectors are pT ordered
  std::vector<int> wLeptons;
  int wNeutrino = -1;
  int wLepton = -1;
  std::vector<int> zLeptons;
  //std::vector<int> jets = jets(vvEvent);

  std::cout << "Sizes w,z " << ws.size() << " " << zs.size() << std::endl;
  if (ws.size()==1&&zs.size()==1) {
    std::cout.flush();
    std::cout << "Have we found a WZ event?" << std::endl;
    std::cout.flush();
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
  
  if (wzEvent) {
    return WZAnalysis(wzEvent,vvEvent,ws[0],zs[0],zLeptons,wLeptons,wNeutrino,wLepton,jets(vvEvent));
  } else {
    return WZAnalysis(vvEvent);
  }
  
}

  

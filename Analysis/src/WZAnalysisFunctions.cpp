#include "DataObjects/include/Particle.h"
#include "DataObjects/include/VVEvent.h"
#include "Analysis/include/WZAnalysis.h"
#include "Analysis/include/WZAnalysisFunctions.h"


const vvana::WZAnalysis vvana::BuildWZAnalysis(const VVEvent& vvEvent){

  return WZAnalysis(vvEvent);
  
}

  

#include "DataObjects/include/Particle.h"
#include "DataObjects/include/VVEvent.h"
#include "Analysis/include/WZAnalysis.h"


vvana::WZAnalysis::WZAnalysis(const VVEvent& vvEvent):
  _wzEvent(false),_vvEvent(vvEvent)
{
  _wzEvent = false;

}

  

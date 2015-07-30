#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "DataObjects/include/Particle.h"
#include "DataObjects/include/VVEvent.h"
#include "Analysis/include/WZAnalysis.h"

#include <vector>
#include <algorithm>
#include <iostream>

vvana::WZAnalysis::WZAnalysis(const VVEvent& vvEvent):
  _wzEvent(false),_vvEvent(vvEvent)
{
  _wzEvent = false;

}

  

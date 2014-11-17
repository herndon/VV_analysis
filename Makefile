CXX           = g++
CXXFLAGS      = $(OPT2) $(ROOTCFLAGS) -Wall -g -O2 -std=c++11
LD            = g++
LDFLAGS       = $(OPT2)

SRCS = LHEWeights.cpp WZEvent.cpp WZEventsTracker.cpp WZPlots.cpp WpZ_ana.cpp ParticleVector.cpp WZEventList.cpp

ROOTINCLUDES = -I$(ROOTSYS)/include
ROOTLDLIBS = `$(ROOTSYS)/bin/root-config --cflags --glibs` -lEG

INCLUDES = -I. $(ROOTINCLUDES)
LDLIBS = -L. $(ROOTLDLIBS)
EXROOTLIB = -L./ExRootAnalysis/lib -lExRootAnalysis

all : WpZ_ana
	@echo Built $@

.PHONY: clean

clean :
	rm -rf *.o

WpZ_ana: $(SRCS:.cpp=.o)
	$(LD) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $^ $(LDLIBS) $(EXROOTLIB) -o $@
	@echo Built $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -fPIC $^ 
	@echo Compiled $@


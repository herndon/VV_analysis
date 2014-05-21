CXX           = g++
CXXFLAGS      = $(OPT2) $(ROOTCFLAGS) -Wall -O2 -std=c++11
LD            = g++
LDFLAGS       = $(OPT2)

SRCS = WpZ_ana.C

ROOTINCLUDES = -I$(ROOTSYS)/include
ROOTLDLIBS = `$(ROOTSYS)/bin/root-config --cflags --glibs` -lEG

INCLUDES = -I. $(ROOTINCLUDES)
LDLIBS = -L. $(ROOTLDLIBS)
EXROOTLIB = -L./lib -lExRootAnalysis

all : WpZ_ana
	@echo Built $@

.PHONY: clean

clean :
	rm -rf *.o

WpZ_ana: $(SRCS:.cc=.o)
	$(LD) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $^ $(LDLIBS) $(EXROOTLIB) -o $@
	@echo Built $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -fPIC $^
	@echo Compiled $@

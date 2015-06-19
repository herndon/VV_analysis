vpath %.cpp = $(wildcard */src)
LIB_PATH = lib/
MLIB_PATH = mlib/
TEST_PATH = test/
OBJSRCS =  $(wildcard */src/*.cpp)
OBJS = $(addprefix  $(LIB_PATH),$(notdir $(OBJSRCS:.cpp=.o)))
MAINEXES = $(basename $(notdir  $(wildcard */test/*.cpp)))
TESTTARGETS = $(addsuffix  .test,$(MAINEXES))

INCDIRS = -I.
CC = g++
#DEBUG = -g -O0
DEBUG = -g -O3 -fno-omit-frame-pointer -DNDEBUG
CFLAGS = -std=c++11 -Wall $(DEBUG) `root-config --cflags`
LFLAGS = -std=c++11 -Wall $(DEBUG) `root-config --glibs` 

.PRECIOUS:$(LIB_PATH)%.o

all: $(MAINEXES)

%: $(MLIB_PATH)%.o $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) lib/libExRootAnalysis.so $< -o $@

-include  $(LIB_PATH)*.d

$(LIB_PATH)%.o: %.cpp
	$(CC) $(CFLAGS) -MMD -c $(INCDIRS) $< -o $@

$(MLIB_PATH)%.o: */test/%.cpp
	$(CC) $(CFLAGS) -MMD -c $(INCDIRS) $< -o $@

test: $(TESTTARGETS)

%.test: 
	./$* 
	diff -qs $(TEST_PATH)log_$* log_$*

clean:
	\rm *~ */*~ */*/*~ $(LIB_PATH)/*.o $(LIB_PATH)/*.d $(MLIB_PATH)/*.o $(MLIB_PATH)/*.d vvAna log*

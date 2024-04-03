ROOTCFLAGS    = $(shell root-config --cflags) # =:define, include same as the next line
#RATCFLAGS     = -pthread -m64 -I/home/shuaioy/RAT/rat/include
RATCFLAGS     = -pthread -m64 -I/home/shuaioy/RAT/7.0.15/rat/include -I/home/shuaioy/RAT/7.0.15/rat/src 
ROOTLIBS      = $(shell root-config --libs)   -lNew -lMinuit #-L
ROOTGLIBS     = $(shell root-config --glibs)
RATLIBS       = $(ROOTLIBS) -L`geant4-config --prefix`/lib64/ `geant4-config --libs` -L$(RATROOT)/lib -lrat_$(RATSYSTEM) -lpqrat_$(RATSYSTEM) $(shell curl-config --libs) -lbz2

MYOS := $(subst -,,$(shell uname))


ifeq ($(MYOS),Linux)
	  CXX      = g++
	  CXXFLAGS = -g  -Wall -fPIC -DEXE
	  CXXFLAGS     += $(ROOTCFLAGS)
	  CXXFLAGS     += $(RATCFLAGS) #add
	  OPTIM=   -fexpensive-optimizations -funroll-loops
	  LIBS          = $(ROOTLIBS)
	  LIBS          += $(RATLIBS) #add 
	  GLIBS         = $(ROOTGLIBS)
	  GLIBS += -lSpectrum
	      
endif


ifeq ($(MYOS),Darwin)
	  CXX = g++
	  CXXFLAGS = -O2 -Wall -fPIC -DEXE
	  CXXFLAGS     += $(ROOTCFLAGS)
	  OPTIM=   -fexpensive-optimizations -funroll-loops
	  LIBS          = $(ROOTLIBS)
	  GLIBS         = $(ROOTGLIBS)
	  LDFLAGS = -bind_at_load
endif

%.o : %.C
	        $(CXX) -c $(CXXFLAGS) $(IFLAGS) $(OPTIM) -o $*.o $(*F).C
%.o : %.c
	        cc -c $(CFLAGS) $(OPTIM) $(INCPATH) $*.c 
%.o : %.f
	        $(FC) -c $(FFLAGS) $(OPTIM) $(INCPATH) $*.f 
%.o : %.F
	        $(FC) -c $(FFLAGS) $(OPTIM) $(INCPATH) $*.F 



#+CXXFLAGS += -I$(RATROOT)/include
#TaggingCoincidencePair: TaggingCoincidencePair.o 
#	        $(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $@.o $(LIBS) $(GLIBS)
#TaggingCoincidencePair.o: TaggingCoincidencePair.C

TaggingCoincidencePair_Run: TaggingCoincidencePair_Run.o
		$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $@.o $(LIBS) $(GLIBS)
TaggingCoincidencePair_Run.o: TaggingCoincidencePair_Run.C

clean:
	@rm -f *.o core


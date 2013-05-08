
PROG=main
OBJS=gmlparser.o core.o main.o

CXXFLAGS=-O3 -frounding-math -Wall -std=c++11
LIBS=-lCGAL_Core -lCGAL -lboost_thread-mt

all : main

main : $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LIBS) asm-xml.o

clean :
	rm -rf $(PROG) $(OBJS)

.PHONY : all main clean

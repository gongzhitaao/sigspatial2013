
CXXFLAGS=-g -O2 -fstack-protector --param=ssp-buffer-size=4 -Wformat -Werror=format-security -D_FORTIFY_SOURCE=2  -frounding-math -O3 -DNDEBUG -std=c++11

CXXLIBS=-rdynamic -lCGAL_Core -lCGAL -lboost_thread-mt

all : test

test : test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $(CXXLIBS) $<

clean :
	rm -rf test

.PHONE : test all

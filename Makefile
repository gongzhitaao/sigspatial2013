
OBJ_DIR=obj
OBJS=$(addprefix $(OBJ_DIR)/,gmlparser.o core.o)

CXXFLAGS=-frounding-math -Wall -std=c++11 -g
LDLIBS=-lCGAL_Core -lCGAL -lboost_thread-mt -ltbb

.PHONY : all clean main core.test pip.test

all : main

main : main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ main.cpp $(OBJS) asm-xml.o $(LDLIBS)

core.test : test/core.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o a.out test/core.cpp \
	$(OBJS) asm-xml.o $(LDLIBS) -lgtest -lgtest_main

pip.test : test/pip.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o a.out test/pip.cpp \
	$(OBJS) asm-xml.o $(LDLIBS) -lgtest -lgtest_main

$(OBJ_DIR)/%.o : %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(OBJS) : | $(OBJ_DIR)

$(OBJ_DIR) :
	mkdir -p $(OBJ_DIR)

clean :
	rm -rf main a.out $(OBJ_DIR)

# Time-stamp: <2013-06-02 23:32:10 CDT gongzhitaao>

OBJ_DIR=obj
TEST_DIR=test
SRC=$(filter-out main.cpp,$(shell ls *.cpp))
OBJS=$(addprefix $(OBJ_DIR)/,$(patsubst %.cpp,%.o,$(SRC)))

CORE_TEST=inside within parser asmxml memory
OTHER_TEST=bst pip general stress tbb

CXXFLAGS=-frounding-math -Wall -std=c++11 -O2
LDLIBS=-lCGAL_Core -lCGAL -ltbb -pthread
GTEST=-lgtest -lgtest_main

.PHONY : all clean main

all : main

main : main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(OBJS) asm-xml.o $(LDLIBS)

test.leak : a.out
	valgrind --tool=memcheck \
		--track-origins=yes --leak-check=full --log-file=report ./a.out

test.usage : a.out
	valgrind --tool=massif \
		--massif-out-file=report --time-unit=B --stacks=yes ./a.out

define core.test
$(1).test : $(2) $(OBJS)
	$(CXX) $(CXXFLAGS) -o a.out $(2) \
		$(OBJS) asm-xml.o $(LDLIBS) $(GTEST)
endef

define other.test
$(1).test : $(2)
	$(CXX) $(CXXFLAGS) -o a.out $(2) -ltbb -pthread $(GTEST)
endef

$(foreach t,$(CORE_TEST),\
	$(eval $(call core.test,$(t),$(addprefix $(TEST_DIR)/,$(t).cpp))))

$(foreach t,$(OTHER_TEST),\
	$(eval $(call other.test,$(t),$(addprefix $(TEST_DIR)/,$(t).cpp))))

$(OBJ_DIR)/%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJS) : | $(OBJ_DIR)

$(OBJ_DIR) :
	mkdir -p $(OBJ_DIR)

clean :
	rm -rf main a.out $(OBJ_DIR) *.log

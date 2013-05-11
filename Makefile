# Time-stamp: <2013-05-10 20:15:10 CDT gongzhitaao>

OBJ_DIR=obj
TEST_DIR=test/
OBJS=$(addprefix $(OBJ_DIR)/,gmlparser.o core.o)
TEST_CASE=$(shell ls $(TEST_DIR))

CXXFLAGS=-frounding-math -Wall -std=c++11 -g
LDLIBS=-lCGAL_Core -lCGAL -lboost_thread-mt -ltbb

.PHONY : all clean main

all : main

main : main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ main.cpp $(OBJS) asm-xml.o $(LDLIBS)

define test_template
$(1).test : $(2) $(OBJS)
	$(CXX) $(CXXFLAGS) -o a.out $(2) \
		$(OBJS) asm-xml.o $(LDLIBS) -lgtest -lgtest_main
endef

$(foreach t,$(TEST_CASE),\
	$(eval $(call test_template,\
				$(patsubst %.cpp,%,$(t)),\
				$(addprefix $(TEST_DIR),$(t)))))

$(OBJ_DIR)/%.o : %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(OBJS) : | $(OBJ_DIR)

$(OBJ_DIR) :
	mkdir -p $(OBJ_DIR)

clean :
	rm -rf main a.out $(OBJ_DIR)

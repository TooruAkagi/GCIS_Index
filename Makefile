# Allow override of CXX to support multiple compilers (gcc, clang, etc.)
CXX ?= g++
CXXFLAGS = -std=c++17 -g -Wall -Wextra

all : Prog1 Prog2 Prog3 Prog4 Prog5

Prog1: gcis_nep_locate.cpp gcis_types.h
	$(CXX) $(CXXFLAGS) -o gcis_nep_locate gcis_nep_locate.cpp

Prog2: gcis_uni_locate.cpp gcis_types.h
	$(CXX) $(CXXFLAGS) -o gcis_uni_locate gcis_uni_locate.cpp

Prog3: gcis_nep_build.cpp gcis_types.h
	$(CXX) $(CXXFLAGS) -o gcis_nep_build gcis_nep_build.cpp

Prog4: gcis_uni_build.cpp gcis_types.h
	$(CXX) $(CXXFLAGS) -o gcis_uni_build gcis_uni_build.cpp

Prog5: genpattern.cpp gcis_types.h
	$(CXX) $(CXXFLAGS) -o genpattern genpattern.cpp

clean:
	rm -f gcis_nep_locate gcis_uni_locate gcis_nep_build gcis_uni_build genpattern
	rm -f *.index *.txt.tmp query_automake.txt

test: all
	@bash test.sh

.PHONY: all clean test

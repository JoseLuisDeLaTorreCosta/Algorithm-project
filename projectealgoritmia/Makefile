CXX = g++
CXXFLAGS = -g -std=c++14 -O3
EXTRAFLAGS = -Wall -Wno-sign-compare

OBJ = ntuple.o bkdtree.o

all: tester_sequencial tests

#$^ substitueix "main.o $(OBJ)" i $@ substitueix "main"
tester_sequencial: tester_sequencial.o stkdtree/stkdtree.o rkdtree/rkdtree.o sqkdtree/sqkdtree.o $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

tests: tests.o stkdtree/stkdtree.o rkdtree/rkdtree.o sqkdtree/sqkdtree.o $(OBJ)
	$(CXX) $(CXXFLAGS) $(EXTRAFLAGS) $^ -o $@ -pthread

clean:
	+rm ./**/*.o *.o tester_sequencial tests
CXXFLAGS += -std=c++11
CXXFLAGS += -Wall
CXXFLAGS += -lmpi
CXXFLAGS += -lm

# Various folders in the project
BINDIR = $(PWD)/bin
BUILDDIR = $(PWD)/build
HEADERDIR = $(PWD)/include
SRCDIR = $(PWD)/src
TESTDIR = $(PWD)/test


.PHONY : clean distclean

HEADERS := $(HEADERDIR)/quadtree.h $(HEADERDIR)/cell.h $(HEADERDIR)/point.h $(HEADERDIR)/lipschitzfunction.h $(HEADERDIR)/refinementcriterion.h

#SRC = $(TESTDIR)/quadtree_test.cpp $(TESTDIR)/test_everything.cpp

#OBJ = $(BUILDDIR)/quadtree_test.o $(BUILDDIR)/test_everything.o

EXE = $(BINDIR)/quadtree_test $(BINDIR)/test_everything


all : $(EXE)

$(BUILDDIR)/quadtree_test.o : $(TESTDIR)/quadtree_test.cpp $(HEADERS)
	$(CXX) $< -c $(CXXFLAGS) -o $@

$(BUILDDIR)/test_everything.o : $(TESTDIR)/test_everything.cpp $(HEADERS)
	$(CXX) $< -c $(CXXFLAGS) -o $@

$(BINDIR)/quadtree_test : $(BUILDDIR)/quadtree_test.o 
	$(CXX) $< $(CXXFLAGS) -o $@

$(BINDIR)/test_everything : $(BUILDDIR)/test_everything.o
	$(CXX) $< $(CXXFLAGS) -o $@


clean : 
	$(RM) -rf $(BUILDDIR)/*.o

distclean : clean
	$(RM) -rf $(EXE)





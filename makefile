CXXFLAGS += -std=c++11
CXXFLAGS += -Wall
CXXFLAGS += -lmpi

EXE = quadtree_test
#EXE = quadtree_test_film

OBJ = quadtree_test.o
#OBJ = quadtree_test_film.o

all : $(EXE)

.PHONY : clean distclean


quadtree_test.o : quadtree.h cell.h point.h lipschitzfunction.h refinementcriterion.h

$(EXE) : $(OBJ)
	$(CXX) $(OBJ) -o $(EXE)




clean : 
	$(RM) -rf *.o

distclean : clean
	$(RM) -rf $(EXE)





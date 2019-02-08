CXXFLAGS += -std=c++11
CXXFLAGS += -Wall

EXE = quadtree_test

OBJ = quadtree_test.o

all : $(EXE)

.PHONY : clean distclean


quadtree_test.o : quadtree.h cell.h point.h lipschitzfunction.h

$(EXE) : $(OBJ)
	$(CXX) $(OBJ) -o $(EXE)




clean : 
	$(RM) -rf *.o

distclean : clean
	$(RM) -rf $(EXE)





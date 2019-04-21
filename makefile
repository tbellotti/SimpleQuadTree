# THINK AT THE DEPENDENCIES: CELL MUST BE INCLUDED EVERYWHERE FOR EXAMPLE.
CXXFLAGS += -Wall -std=c++14 \
   -Werror \
   -Wextra \
   -Wconversion \
   -Wno-deprecated \
   -Winit-self \
   -Wsign-conversion \
   -Wredundant-decls \
   -Wvla -Wshadow -Wctor-dtor-privacy -Wnon-virtual-dtor -Woverloaded-virtual \
   -Winit-self \
   -Wpointer-arith \
   -Wcast-qual \
   -Wcast-align \
   -Wdouble-promotion \
   -Wold-style-cast -Wno-error=old-style-cast \
   -Wsign-promo \
   -Wswitch-enum \
   -Wswitch-default \
   -Wundef

MPICOMP = mpicxx

# Various folders in the project
BINDIR = $(PWD)/bin
BUILDDIR = $(PWD)/build
HEADERDIR = $(PWD)/include
SRCDIR = $(PWD)/src
TESTDIR = $(PWD)/test

.PHONY : clean distclean

HEADERS := $(HEADERDIR)/quadtree.h $(HEADERDIR)/cell.h $(HEADERDIR)/point.h $(HEADERDIR)/lipschitzfunction.h $(HEADERDIR)/refinementcriterion.h $(HEADERDIR)/levelsetcriterion.h $(HEADERDIR)/rgbcolor.h

#SRC = $(TESTDIR)/quadtree_test.cpp $(TESTDIR)/test_everything.cpp

#OBJ = $(BUILDDIR)/quadtree_test.o $(BUILDDIR)/test_everything.o

#EXE = $(BINDIR)/quadtree_test $(BINDIR)/test_everything $(BINDIR)/split_parallel_tree_integration $(BINDIR)/circle_parallel_quadrature

EXE = $(BINDIR)/test/basic_bricks $(BINDIR)/test/image_compression $(BINDIR)/test/quadtree_highly_adaptive_mesh $(BINDIR)/test/quadtree_other_meshes $(BINDIR)/test/quadtree_integration $(BINDIR)/imagecompression $(BINDIR)/test/quadtree_time_evolving_mesh $(BINDIR)/mpi_quadtree_pi

all : $(EXE)

# Compiling RGBCOLOR
$(BUILDDIR)/rgbcolor.o : $(SRCDIR)/rgbcolor.cpp $(HEADERDIR)/matrix.h
	$(CXX) $< -c $(CXXFLAGS) -o $@

# Compiling the BASIC_BRICKS example

$(BUILDDIR)/basic_bricks.o : $(SRCDIR)/test/basic_bricks.cpp $(HEADERDIR)/point.h $(HEADERDIR)/rgbcolor.h $(HEADERDIR)/matrix.h
	$(CXX) $< -c $(CXXFLAGS) -o $@

$(BINDIR)/test/basic_bricks : $(BUILDDIR)/basic_bricks.o $(BUILDDIR)/rgbcolor.o 
	$(CXX) $^ $(CXXFLAGS) -o $@

# Compiling the IMAGE_COMPRESSION example

$(BUILDDIR)/image_compression.o : $(SRCDIR)/test/image_compression.cpp $(HEADERDIR)/image.h 
	$(CXX) $< -c $(CXXFLAGS) -o $@

$(BINDIR)/test/image_compression : $(BUILDDIR)/image_compression.o $(BUILDDIR)/rgbcolor.o 
	$(CXX) $^ $(CXXFLAGS) -o $@

# Compiling the QUADTREE_HIGHLY_ADAPTIVE_MESH example

$(BUILDDIR)/quadtree_highly_adaptive_mesh.o : $(SRCDIR)/test/quadtree_highly_adaptive_mesh.cpp $(HEADERDIR)/quadtree.h 
	$(CXX) $< -c $(CXXFLAGS) -o $@

$(BINDIR)/test/quadtree_highly_adaptive_mesh : $(BUILDDIR)/quadtree_highly_adaptive_mesh.o $(BUILDDIR)/rgbcolor.o 
	$(CXX) $^ $(CXXFLAGS) -o $@

# Compiling the QUADTREE_OTHER_MESHES example

$(BUILDDIR)/quadtree_other_meshes.o : $(SRCDIR)/test/quadtree_other_meshes.cpp $(HEADERDIR)/quadtree.h
	$(CXX) $< -c $(CXXFLAGS) -o $@

$(BINDIR)/test/quadtree_other_meshes : $(BUILDDIR)/quadtree_other_meshes.o $(BUILDDIR)/rgbcolor.o 
	$(CXX) $^ $(CXXFLAGS) -o $@


# Compiling the QUADTREE_INTEGRATION example

$(BUILDDIR)/quadtree_integration.o : $(SRCDIR)/test/quadtree_integration.cpp $(HEADERDIR)/quadtree.h 
	$(CXX) $< -c $(CXXFLAGS) -o $@

$(BINDIR)/test/quadtree_integration : $(BUILDDIR)/quadtree_integration.o $(BUILDDIR)/rgbcolor.o 
	$(CXX) $^ $(CXXFLAGS) -o $@

# Compiling the IMAGECOMPRESSION program
$(BUILDDIR)/imagecompression.o : $(SRCDIR)/main/imagecompression.cpp $(HEADERDIR)/image.h 
	$(CXX) $< -c $(CXXFLAGS) -o $@

$(BINDIR)/imagecompression : $(BUILDDIR)/imagecompression.o $(BUILDDIR)/rgbcolor.o 
	$(CXX) $^ $(CXXFLAGS) -o $@


# Compiling the quadtree_time_evolving_mesh test
$(BUILDDIR)/quadtree_time_evolving_mesh.o : $(SRCDIR)/test/quadtree_time_evolving_mesh.cpp $(HEADERDIR)/quadtree.h 
	$(CXX) $< -c $(CXXFLAGS) -o $@

$(BINDIR)/test/quadtree_time_evolving_mesh : $(BUILDDIR)/quadtree_time_evolving_mesh.o $(BUILDDIR)/rgbcolor.o 
	$(CXX) $^ $(CXXFLAGS) -o $@

# Compiling the MPI_QUADTREE_PI program
$(BUILDDIR)/mpi_quadtree_pi.o : $(SRCDIR)/main/imagecompression.cpp $(HEADERDIR)/image.h 
	$(CXX) $< -c $(CXXFLAGS) -o $@

$(BINDIR)/mpi_quadtree_pi : $(SRCDIR)/main/mpi_quadtree_pi.cpp $(BUILDDIR)/rgbcolor.o 
	mpicxx $^ -o $@
# I cannot use the flag suggested because they generate too many errors
# with the MPI implementation I use.

clean : 
	$(RM) -rf $(BUILDDIR)/*.o

distclean : clean
	$(RM) -rf $(EXE)





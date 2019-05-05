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
HEADERDIR = $(PWD)/include # Not used.
SRCDIR = $(PWD)/src

.PHONY : clean distclean

# I have to put and update the two since I have a quite complicated way of structuring the 
# folders, thus there is not always a correspondence between folder for the executables
# and the objects/source files.
EXE = $(BINDIR)/test/basic_bricks $(BINDIR)/test/image_compression $(BINDIR)/test/quadtree_highly_adaptive_mesh $(BINDIR)/test/quadtree_other_meshes $(BINDIR)/test/quadtree_integration $(BINDIR)/imagecompression $(BINDIR)/test/quadtree_time_evolving_mesh $(BINDIR)/test/mpi_pi $(BINDIR)/test/mpi_gauss $(BINDIR)/test/mpi_square $(BINDIR)/test/mpi_unif
OBJ = $(BUILDDIR)/test/basic_bricks.o $(BUILDDIR)/test/image_compression.o $(BUILDDIR)/test/quadtree_highly_adaptive_mesh.o $(BUILDDIR)/test/quadtree_other_meshes.o $(BUILDDIR)/test/quadtree_integration.o $(BUILDDIR)/main/imagecompression.o $(BUILDDIR)/test/quadtree_time_evolving_mesh.o $(BUILDDIR)/test/mpi_pi.o $(BUILDDIR)/test/mpi_gauss.o $(BUILDDIR)/test/mpi_square.o $(BUILDDIR)/test/mpi_unif.o

all : $(EXE)

# To have automatic dependencies
-include $(OBJ:.o=.d)


# === Setting general rules, which must be take the fact that we 
# have several subdirectories into account === 

# To compile files like src/*.cpp
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c -MP -MD $(CXXFLAGS) $< -o $@

# To compile files like src/main/*.cpp
$(BUILDDIR)/main/%.o: $(SRCDIR)/main/%.cpp
	$(CXX) -c -MP -MD $(CXXFLAGS) $< -o $@

# To compile files like src/test/*.cpp
$(BUILDDIR)/test/%.o: $(SRCDIR)/test/%.cpp
	$(CXX) -c -MP -MD $(CXXFLAGS) $< -o $@


# === Compiling and linking ===

# Compiling RGBCOLOR
$(BUILDDIR)/rgbcolor.o : $(SRCDIR)/rgbcolor.cpp

# Compiling and linking the BASIC_BRICKS example
$(BUILDDIR)/test/basic_bricks.o : $(SRCDIR)/test/basic_bricks.cpp 
$(BINDIR)/test/basic_bricks : $(BUILDDIR)/test/basic_bricks.o $(BUILDDIR)/rgbcolor.o 
	$(CXX) $^ $(CXXFLAGS) -o $@

# Compiling and linking the IMAGE_COMPRESSION example
$(BUILDDIR)/test/image_compression.o : $(SRCDIR)/test/image_compression.cpp
$(BINDIR)/test/image_compression : $(BUILDDIR)/test/image_compression.o $(BUILDDIR)/rgbcolor.o 
	$(CXX) $^ $(CXXFLAGS) -o $@

# Compiling and linking the QUADTREE_HIGHLY_ADAPTIVE_MESH example
$(BUILDDIR)/test/quadtree_highly_adaptive_mesh.o : $(SRCDIR)/test/quadtree_highly_adaptive_mesh.cpp 
$(BINDIR)/test/quadtree_highly_adaptive_mesh : $(BUILDDIR)/test/quadtree_highly_adaptive_mesh.o $(BUILDDIR)/rgbcolor.o 
	$(CXX) $^ $(CXXFLAGS) -o $@

# Compiling and linking the QUADTREE_OTHER_MESHES example
$(BUILDDIR)/test/quadtree_other_meshes.o : $(SRCDIR)/test/quadtree_other_meshes.cpp
$(BINDIR)/test/quadtree_other_meshes : $(BUILDDIR)/test/quadtree_other_meshes.o $(BUILDDIR)/rgbcolor.o 
	$(CXX) $^ $(CXXFLAGS) -o $@

# Compiling and linking the QUADTREE_INTEGRATION example
$(BUILDDIR)/test/quadtree_integration.o : $(SRCDIR)/test/quadtree_integration.cpp
$(BINDIR)/test/quadtree_integration : $(BUILDDIR)/test/quadtree_integration.o $(BUILDDIR)/rgbcolor.o 
	$(CXX) $^ $(CXXFLAGS) -o $@

# Compiling and linking the IMAGECOMPRESSION program
$(BUILDDIR)/main/imagecompression.o : $(SRCDIR)/main/imagecompression.cpp 
$(BINDIR)/imagecompression : $(BUILDDIR)/main/imagecompression.o $(BUILDDIR)/rgbcolor.o 
	$(CXX) $^ $(CXXFLAGS) -o $@

# Compiling and linking the quadtree_time_evolving_mesh test
$(BUILDDIR)/test/quadtree_time_evolving_mesh.o : $(SRCDIR)/test/quadtree_time_evolving_mesh.cpp
$(BINDIR)/test/quadtree_time_evolving_mesh : $(BUILDDIR)/test/quadtree_time_evolving_mesh.o $(BUILDDIR)/rgbcolor.o 
	$(CXX) $^ $(CXXFLAGS) -o $@

# I cannot use the flag suggested because they generate too many errors
# with the MPI implementation I use.

# Compiling and linking the MPI_PI test
$(BUILDDIR)/test/mpi_pi.o : $(SRCDIR)/test/mpi_pi.cpp
	mpicxx -c -MP -MD $< -o $@
$(BINDIR)/test/mpi_pi : $(BUILDDIR)/test/mpi_pi.o $(BUILDDIR)/rgbcolor.o 
	mpicxx $^  -o $@

# Compiling and linking the MPI_GAUSS test
$(BUILDDIR)/test/mpi_gauss.o : $(SRCDIR)/test/mpi_gauss.cpp
	mpicxx -c -MP -MD $< -o $@
$(BINDIR)/test/mpi_gauss : $(BUILDDIR)/test/mpi_gauss.o $(BUILDDIR)/rgbcolor.o 
	mpicxx $^  -o $@

# Compiling and linking the MPI_SQUARE test
$(BUILDDIR)/test/mpi_square.o : $(SRCDIR)/test/mpi_square.cpp
	mpicxx -c -MP -MD $< -o $@
$(BINDIR)/test/mpi_square : $(BUILDDIR)/test/mpi_square.o $(BUILDDIR)/rgbcolor.o 
	mpicxx $^  -o $@

# Compiling and linking the MPI_UNIF test
$(BUILDDIR)/test/mpi_unif.o : $(SRCDIR)/test/mpi_unif.cpp
	mpicxx -c -MP -MD $< -o $@
$(BINDIR)/test/mpi_unif : $(BUILDDIR)/test/mpi_unif.o $(BUILDDIR)/rgbcolor.o 
	mpicxx $^  -o $@

clean : 
	$(RM) -rf $(OBJ) $(OBJ:.o=.d)

distclean : clean
	$(RM) -rf $(EXE)





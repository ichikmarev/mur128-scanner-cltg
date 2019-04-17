LINKER      = g++
LINKERFLAGS = -s
CXX         = g++
CXXFLAGS    = -O3 -Wall -std=c++14 -Wextra
BIN         = table-gen
vpath %.o build
OBJ         = table-gen.o char_conv.o create_permutation.o create_permutation_tree.o format.o list_to_columns.o permutation_tree_to_permutation.o
LINKOBJ     = build/table-gen.o build/char_conv.o build/create_permutation.o build/create_permutation_tree.o build/format.o build/list_to_columns.o build/permutation_tree_to_permutation.o

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean: clean-custom
	rm -f ./build/*.o
	rm -f ./build/$(BIN)

.cpp.o:
	$(CXX) -c $< -o $@ $(CXXFLAGS)
	mv $@ ./build

$(BIN):$(OBJ)
	$(LINKER) -o $(BIN) $(LINKOBJ) $(LINKERFLAGS)
	mv $(BIN) ./build
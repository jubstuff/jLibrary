C_COMPILER=gcc
OPTIONS=-Wall -g

LIST_BASE=test_list
SET_BASE=test_set
VERTEX_BASE=test_vertex
GRAPH_BASE=test_graph
ARRAY_BASE=test_array
ESET_BASE=test_eset
MATRIX_BASE=test_matrix

ifeq ($(OS),Windows_NT)
	TARGET_EXTENSION=.exe
else
	TARGET_EXTENSION=.out
endif

LIST_TARGET = $(LIST_BASE)$(TARGET_EXTENSION)
SET_TARGET = $(SET_BASE)$(TARGET_EXTENSION)
VERTEX_TARGET = $(VERTEX_BASE)$(TARGET_EXTENSION)
GRAPH_TARGET = $(GRAPH_BASE)$(TARGET_EXTENSION)
ARRAY_TARGET = $(ARRAY_BASE)$(TARGET_EXTENSION)
ESET_TARGET = $(ESET_BASE)$(TARGET_EXTENSION)
MATRIX_TARGET = $(MATRIX_BASE)$(TARGET_EXTENSION)

LIST_SRC_FILES=TestList.c TestList_Runner.c
SET_SRC_FILES=TestSet.c TestSet_Runner.c
VERTEX_SRC_FILES=TestVertex.c TestVertex_Runner.c
GRAPH_SRC_FILES=TestGraph.c TestGraph_Runner.c
ARRAY_SRC_FILES=TestArray.c TestArray_Runner.c
ESET_SRC_FILES=TestEset.c TestEset_Runner.c
MATRIX_SRC_FILES=TestMatrix.c TestMatrix_Runner.c

INC_DIRS=-I../src -I./unity/src
SYMBOLS=-DTEST -DDEBUG

ifeq ($(OS),Windows_NT)
	CLEANUP = del /F /Q build\* &&  del /F /Q $(TARGET3) 
else
	CLEANUP = rm -f build/*.o ; rm -f $(TARGET3) 
endif
#dove cercare i file sorgente
vpath %.c unity/src ../src
vpath %.h unity/src ../src

all: list set vertex

list: unity.o jMem.o jList.o
	ruby ./unity/auto/generate_test_runner.rb TestList.c TestList_Runner.c
	$(C_COMPILER) $(OPTIONS) $(INC_DIRS) $(SYMBOLS) $^ $(LIST_SRC_FILES) -o $(LIST_TARGET)
	./$(LIST_TARGET)
set: unity.o jMem.o jList.o jSet.o
	ruby ./unity/auto/generate_test_runner.rb TestSet.c TestSet_Runner.c
	$(C_COMPILER) $(OPTIONS) $(INC_DIRS) $(SYMBOLS) $^ $(SET_SRC_FILES) -o $(SET_TARGET)
	./$(SET_TARGET)
vertex: unity.o jMem.o jList.o jVertex.o
	ruby ./unity/auto/generate_test_runner.rb TestVertex.c TestVertex_Runner.c
	$(C_COMPILER) $(OPTIONS) $(INC_DIRS) $(SYMBOLS) $^ $(VERTEX_SRC_FILES) -o $(VERTEX_TARGET)
	./$(VERTEX_TARGET)
graph: unity.o jList.o jMem.o jSet.o jGraph.o jVertex.o
	ruby ./unity/auto/generate_test_runner.rb TestGraph.c TestGraph_Runner.c
	$(C_COMPILER) $(OPTIONS) $(INC_DIRS) $(SYMBOLS) $^ $(GRAPH_SRC_FILES) -o $(GRAPH_TARGET)
	./$(GRAPH_TARGET)
array: unity.o jMem.o jList.o jArray.o errors.h common.h
	ruby ./unity/auto/generate_test_runner.rb TestArray.c TestArray_Runner.c
	$(C_COMPILER) $(OPTIONS) $(INC_DIRS) $(SYMBOLS) $^ $(ARRAY_SRC_FILES) -o $(ARRAY_TARGET)
	./$(ARRAY_TARGET)
matrix: unity.o jMem.o errors.h common.h jMatrix.o jVertex.o
	ruby ./unity/auto/generate_test_runner.rb TestMatrix.c TestMatrix_Runner.c
	$(C_COMPILER) $(OPTIONS) $(INC_DIRS) $(SYMBOLS) $^ $(MATRIX_SRC_FILES) -o $(MATRIX_TARGET)
	./$(MATRIX_TARGET)
eset: unity.o jMem.o jList.o jArray.o errors.h common.h jEdgeSet.o jVertex.o jMatrix.o
	ruby ./unity/auto/generate_test_runner.rb TestEset.c TestEset_Runner.c
	$(C_COMPILER) $(OPTIONS) $(INC_DIRS) $(SYMBOLS) $^ $(ESET_SRC_FILES) -o $(ESET_TARGET)
	./$(ESET_TARGET)


jEdgeSet.o: jEdgeSet.c jEdgeSet.h 
	$(C_COMPILER) $(OPTIONS) -c $^
jMatrix.o: jMatrix.c jMatrix.h common.h
	$(C_COMPILER) $(OPTIONS) -c $^
jArray.o: jArray.c jArray.h errors.h
	$(C_COMPILER) $(OPTIONS) -c $^
jGraph.o: jGraph.c jGraph.h
	$(C_COMPILER) $(OPTIONS) -c $^
jVertex.o: jVertex.c jVertex.h
	$(C_COMPILER) $(OPTIONS) -c $^
jSet.o: jSet.c jSet.h
	$(C_COMPILER) $(OPTIONS) -c $^
jList.o: jList.c jList.h errors.h common.h jMem.o
	$(C_COMPILER) $(OPTIONS) -c $^
jMem.o: jMem.c jMem.h errors.h
	$(C_COMPILER) $(OPTIONS) -c $^
unity.o: unity.c unity.h
	$(C_COMPILER) $(OPTIONS) -c $^

clean:
	rm *.o	

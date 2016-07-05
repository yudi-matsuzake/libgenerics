GCC=gcc
GCC_FLAGS=-Wall -Wextra
OBJ=build/obj

INCLUDE=include
SRC_PATH=src
SRC=$(wildcard $(SRC_PATH)/*.c)
HEADER=$(wildcard $(INCLUDE)/*.h)

EXAMPLES_PATH=doc/examples
EXAMPLES=$(wildcard $(EXAMPLES_PATH)/*.c)
EXAMPLES_BIN=build/examples

TEST_PATH=build/test

generics-obj: $(SRC) $(HEADER)
	@[ -d $(OBJ) ] || mkdir -p $(OBJ)
	$(GCC) -c $(SRC) -I $(INCLUDE)
	mv *.o $(OBJ)

examples: generics-obj
	@[ -d $(EXAMPLES_BIN) ] || mkdir -p $(EXAMPLES_BIN)
	@for example in $(EXAMPLES_PATH)/*.c ;\
	do \
		bin=$$(basename $${example}) ;\
		echo $(GCC) $${example} -o $(EXAMPLES_BIN)/$${bin%.c} -I $(INCLUDE) $(OBJ)/*.o ;\
		$(GCC) $${example} -o $(EXAMPLES_BIN)/$${bin%.c} -I $(INCLUDE) $(OBJ)/*.o ;\
	done

test: examples
	@[ -d  $(TEST_PATH) ] || mkdir -p $(TEST_PATH)
	@for bin in $(EXAMPLES_BIN)/* ;\
	do \
		test=$$(basename $${bin}) ;\
		echo valgrind ./$${bin} '1>' $(TEST_PATH)/$${test}_output.txt '2>' $(TEST_PATH)/$${test}_valgrind.txt ;\
		valgrind ./$${bin} 1> $(TEST_PATH)/$${test}_output.txt 2> $(TEST_PATH)/$${test}_valgrind.txt ;\
	done

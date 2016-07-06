GCC=gcc
GCC_FLAGS=-Wall -Wextra -O3
OBJ=build/obj

INCLUDE=include
SRC_PATH=src
SRC=$(wildcard $(SRC_PATH)/*.c)
HEADER=$(wildcard $(INCLUDE)/*.h)

EXAMPLES_PATH=doc/examples
EXAMPLES=$(wildcard $(EXAMPLES_PATH)/*.c)
EXAMPLES_BIN=build/examples

TEST_PATH=build/test

DOXYGEN_PATH=doc/doxygen

$(OBJ): $(SRC) $(HEADER)
	@[ -d $(OBJ) ] || mkdir -p $(OBJ)
	$(GCC) -c $(SRC) -I $(INCLUDE) $(GCC_FLAGS)
	mv *.o $(OBJ)

$(EXAMPLES_BIN): $(OBJ)
	@[ -d $(EXAMPLES_BIN) ] || mkdir -p $(EXAMPLES_BIN)
	@for example in $(EXAMPLES_PATH)/*.c ;\
	do \
		bin=$$(basename $${example}) ;\
		echo $(GCC) $${example} -o $(EXAMPLES_BIN)/$${bin%.c} -I $(INCLUDE) $(OBJ)/*.o ;\
		$(GCC) $${example} -o $(EXAMPLES_BIN)/$${bin%.c} -I $(INCLUDE) $(OBJ)/*.o $(GCC_FLAGS) ;\
	done

$(TEST_PATH): $(EXAMPLES_BIN)
	@[ -d  $(TEST_PATH) ] || mkdir -p $(TEST_PATH)
	@for bin in $(EXAMPLES_BIN)/* ;\
	do \
		test=$$(basename $${bin}) ;\
		echo valgrind ./$${bin} '1>' $(TEST_PATH)/$${test}_output.txt '2>' $(TEST_PATH)/$${test}_valgrind.txt ;\
		valgrind ./$${bin} 1> $(TEST_PATH)/$${test}_output.txt 2> $(TEST_PATH)/$${test}_valgrind.txt ;\
	done
	@for txt in $(TEST_PATH)/*.txt ;\
	do \
		echo "$${txt}--------------------------------------" ;\
		cat "$${txt}" ;\
	done | less

$(DOXYGEN_PATH): .doxygen
	doxygen .doxygen

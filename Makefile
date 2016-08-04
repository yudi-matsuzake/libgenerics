GCC=gcc
GCC_FLAGS=-Wall -Wextra -O3 -lgenerics
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

DOXYGEN_REFMAN=$(DOXYGEN_PATH)/latex/refman.pdf

DOC_PDF=doc/doc.pdf

LIB_STATIC_PATH=build/lib/static
LIB_STATIC=$(LIB_STATIC_PATH)/libgenerics.a
LIB_SHARED_PATH=build/lib/shared
LIB_SHARED=$(LIB_SHARED_PATH)/libgenerics.so
OBJ_SHARED=build/obj-shared

INSTALL_ARTEFACT_PATH=/usr/lib
INSTALL_INCLUDE_PATH=/usr/include/generics

all: $(LIB_STATIC) $(LIB_SHARED)

$(OBJ): $(SRC) $(HEADER)
	@[ -d $(OBJ) ] || mkdir -p $(OBJ)
	$(GCC) -c $(SRC) -I $(INCLUDE) $(GCC_FLAGS)
	mv *.o $(OBJ)

$(EXAMPLES_BIN): $(OBJ)
	@[ -d $(EXAMPLES_BIN) ] || mkdir -p $(EXAMPLES_BIN)
	@for example in $(EXAMPLES_PATH)/*.c ;\
	do \
		bin=$$(basename $${example}) ;\
		echo $(GCC) $${example} -o $(EXAMPLES_BIN)/$${bin%.c} $(GCC_FLAGS) ;\
		$(GCC) $${example} -o $(EXAMPLES_BIN)/$${bin%.c} $(GCC_FLAGS) ;\
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

$(DOXYGEN_PATH): .doxygen $(SRC) $(HEADER)
	doxygen .doxygen

$(DOXYGEN_REFMAN): $(DOXYGEN_PATH)
	cd $(DOXYGEN_PATH)/latex && make

$(DOC_PDF): $(DOXYGEN_REFMAN)
	cp $(DOXYGEN_PATH)/latex/refman.pdf $(DOC_PDF)

$(LIB_STATIC): $(OBJ) $(OBJ)/*.o
	@[ -d $(LIB_STATIC_PATH) ] || mkdir -p $(LIB_STATIC_PATH)
	ar -cvq $(LIB_STATIC) $(OBJ)/*.o
	
$(OBJ_SHARED): $(SRC) $(HEADER)
	@[ -d $(OBJ_SHARED) ] || mkdir -p $(OBJ_SHARED)
	$(GCC) -fPIC -c $(SRC) -I $(INCLUDE) $(GCC_FLAGS)
	mv *.o $(OBJ_SHARED)

$(LIB_SHARED): $(OBJ_SHARED)
	@[ -d $(LIB_SHARED_PATH) ] || mkdir -p $(LIB_SHARED_PATH)
	gcc -shared -Wl,-soname,libgenerics.so -o $(LIB_SHARED) $(OBJ_SHARED)/*.o

install: $(LIB_STATIC) $(LIB_SHARED)
	@[ -d $(INSTALL_INCLUDE_PATH) ] || mkdir -p $(INSTALL_INCLUDE_PATH)
	cp $(HEADER) $(INSTALL_INCLUDE_PATH)
	cp $(LIB_SHARED) $(LIB_STATIC) $(INSTALL_ARTEFACT_PATH)

uninstall:
	-rm -vr $(INSTALL_INCLUDE_PATH)
	rm -v $(INSTALL_ARTEFACT_PATH)/libgenerics.a $(INSTALL_ARTEFACT_PATH)/libgenerics.so

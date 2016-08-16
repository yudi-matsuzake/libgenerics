# --VARIABLES----------------------------------------------
# gcc
GCC=gcc
GCC_FLAGS=-Wall -Wextra -O3

# paths and files
BUILD_PATH=build
LIB_PATH=$(BUILD_PATH)/lib
INCLUDE=include
SRC_PATH=src
SRC=$(wildcard $(SRC_PATH)/*.c)
HEADER=$(wildcard $(INCLUDE)/*.h)

LIB_STATIC_PATH=$(LIB_PATH)/static
LIB_SHARED_PATH=$(LIB_PATH)/shared
STATIC_OBJECT=$(patsubst $(SRC_PATH)/%.c,$(LIB_STATIC_PATH)/%.o,$(SRC))
SHARED_OBJECT=$(patsubst $(SRC_PATH)/%.c,$(LIB_SHARED_PATH)/%.o,$(SRC))

# libs
LIB_STATIC=$(LIB_STATIC_PATH)/libgenerics.a
LIB_SHARED=$(LIB_SHARED_PATH)/libgenerics.so

# installation
LIB_STATIC_INSTALL=/usr/lib/libgenerics.a
LIB_SHARED_INSTALL=/usr/lib/libgenerics.so
HEADER_INSTALL=/usr/include/generics

# example
EXAMPLE_PATH=doc/examples
EXAMPLE=$(wildcard $(EXAMPLE_PATH)/*.c)

# --RULES-DEFINITION---------------------------------------
# objects (*.o) rule
define static_object_rule
$(1): $(patsubst $(LIB_STATIC_PATH)/%.o,$(SRC_PATH)/%.c,$(1)) $(patsubst $(LIB_STATIC_PATH)/%.o,$(INCLUDE)/%.h,$(1))
	@mkdir -p $(LIB_STATIC_PATH)
	$(GCC) $(GCC_FLAGS) -c -o $(1) $(patsubst $(LIB_STATIC_PATH)/%.o,$(SRC_PATH)/%.c,$(1)) -I $(INCLUDE)
endef

define shared_object_rule
$(1): $(patsubst $(LIB_SHARED_PATH)/%.o,$(SRC_PATH)/%.c,$(1)) $(patsubst $(LIB_SHARED_PATH)/%.o,$(INCLUDE)/%.h,$(1))
	@mkdir -p $(LIB_SHARED_PATH)
	$(GCC) $(GCC_FLAGS) -fPIC -c -o $(1) $(patsubst $(LIB_SHARED_PATH)/%.o,$(SRC_PATH)/%.c,$(1)) -I $(INCLUDE)
endef

# --RULES--------------------------------------------------
all: $(LIB_STATIC) $(LIB_SHARED)

$(foreach obj, $(STATIC_OBJECT), $(eval $(call static_object_rule, $(obj))))
$(foreach obj, $(SHARED_OBJECT), $(eval $(call shared_object_rule, $(obj))))

$(LIB_STATIC): $(STATIC_OBJECT)
	ar -cvq $(LIB_STATIC) $(STATIC_OBJECT)

$(LIB_SHARED): $(SHARED_OBJECT)
	$(GCC) -shared -Wl,-soname,libgenerics.so -o $(LIB_SHARED) $(SHARED_OBJECT)

# --INSTALATION--------------------------------------------
install: $(LIB_STATIC) $(LIB_SHARED)
	mkdir -p $(HEADER_INSTALL)
	cp -vr $(HEADER) $(HEADER_INSTALL)
	cp $(LIB_STATIC) $(LIB_STATIC_INSTALL)
	cp $(LIB_SHARED) $(LIB_SHARED_INSTALL)

uninstall:
	rm $(patsubst $(INCLUDE)/%,$(HEADER_INSTALL)/%,$(HEADER))
	rmdir $(HEADER_INSTALL)
	rm $(LIB_STATIC_INSTALL)
	rm $(LIB_SHARED_INSTALL)

# --EXAMPLE------------------------------------------------
examples: $(EXAMPLE)
	make -C $(EXAMPLE_PATH) BUILD_PATH=../../build/examples

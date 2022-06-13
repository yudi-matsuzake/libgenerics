# --VARIABLES----------------------------------------------
# gcc
GCC=gcc
GCC_FLAGS=-Wall -Wextra -O3 -I../../include

# paths and files
BUILD_PATH=build/example
EXAMPLE_PATH=.
EXAMPLE=$(wildcard $(EXAMPLE_PATH)/*.c)
STATIC_LIB=../../build/lib/static/libgenerics.a


# --RULES-DEFINITION---------------------------------------
define build_example
$(patsubst $(EXAMPLE_PATH)/%.c,$(BUILD_PATH)/%.out,$(1)): $(1) ${STATIC_LIB}
	@mkdir -p $(BUILD_PATH)
	$(GCC) $(GCC_FLAGS) -o $(patsubst $(EXAMPLE_PATH)/%.c,$(BUILD_PATH)/%.out,$(1)) $(1) ${STATIC_LIB}
endef

# --RULES--------------------------------------------------
all: $(patsubst $(EXAMPLE_PATH)/%.c,$(BUILD_PATH)/%.out,$(EXAMPLE))

$(foreach ex, $(EXAMPLE), $(eval $(call build_example, $(ex))))

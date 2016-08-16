# --VARIABLES----------------------------------------------
# gcc
GCC=gcc
GCC_FLAGS=-Wall -Wextra -O3

# paths and files
BUILD_PATH=build/example
EXAMPLE_PATH=.
EXAMPLE=$(wildcard $(EXAMPLE_PATH)/*.c)


# --RULES-DEFINITION---------------------------------------
define build_example
$(patsubst $(EXAMPLE_PATH)/%.c,$(BUILD_PATH)/%.out,$(1)): $(1)
	@mkdir -p $(BUILD_PATH)
	$(GCC) $(GCC_FLAGS) -o $(patsubst $(EXAMPLE_PATH)/%.c,$(BUILD_PATH)/%.out,$(1)) $(1) -lgenerics
endef

# --RULES--------------------------------------------------
all: $(patsubst $(EXAMPLE_PATH)/%.c,$(BUILD_PATH)/%.out,$(EXAMPLE))

$(foreach ex, $(EXAMPLE), $(eval $(call build_example, $(ex))))

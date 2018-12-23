PROJECT_NAME	:= 3DRender

SOURCE_DIR		:= src/
BIN_DIR			:= bin/
INCLUDE_DIR		:= include/
LIBRARY_DIR		:= lib/

CXX				:= g++
CXXFLAGS		:= -std=c++11 -I$(INCLUDE_DIR) -I$(SOURCE_DIR) -O3 -DGLEW_STATIC=1 -gdwarf-2
LDFLAGS			:= -L$(LIBRARY_DIR)
LDLIBS			:= -lmingw32 -lassimp -lglew32 -lSDL2main -lSDL2 -lopengl32

OPTIMIZE = false
ifeq ($(firstword $(MAKECMDGOALS)),run)
	OPTIMIZE = true
else ifeq ($(firstword $(MAKECMDGOALS)),clean)
	OPTIMIZE = true
else ifeq ($(firstword $(MAKECMDGOALS)),all)
	OPTIMIZE = true
else ifeq ($(firstword $(MAKECMDGOALS)),)
	OPTIMIZE = true
endif

# function for recursive wildcard
rwildcard = $(foreach d,$(wildcard $1*), \
				$(call rwildcard,$d/,$2) \
				$(filter $(subst *,%,$2),$d) \
			)

ifeq ($(OPTIMIZE),false)
	SOURCE_FILES	:= $(call rwildcard, $(SOURCE_DIR), *.cpp *.cc)
	DLLS			:= $(call rwildcard, $(LIBRARY_DIR), *.dll)
	DLLS_OUT		:= $(addprefix $(BIN_DIR),$(notdir $(DLLS)))
	OBJ_FILES		:= $(addsuffix .o,$(patsubst $(SOURCE_DIR)%,$(BIN_DIR)%, $(SOURCE_FILES)))
	DEPENDENCIES	:= $(patsubst $(BIN_DIR)%.o,$(BIN_DIR)%.d, $(OBJ_FILES))
	DIRECTORIES		:= $(BIN_DIR)
	DIRECTORIES		+= $(dir $(OBJ_FILES))
	DIRECTORIES		:= $(sort $(DIRECTORIES))
endif

OUTPUT_FILE		:= $(BIN_DIR)$(PROJECT_NAME)

all:
	+make build
	+make run

build: $(OUTPUT_FILE) $(DLLS_OUT)

$(OUTPUT_FILE): $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS) $(LDLIBS)

-include $(DEPENDENCIES)

.SECONDEXPANSION:
$(OBJ_FILES): $$(patsubst $(BIN_DIR)%,$(SOURCE_DIR)%, $$(basename $$@)) | $$(dir $$@)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

.SECONDEXPANSION:
$(DLLS_OUT): $$(filter %$$(notdir $$@),$(DLLS))
	cp $< $@

$(DIRECTORIES):
	mkdir -p $@

ifeq ($(firstword $(MAKECMDGOALS)),run)
	# use the rest as arguments for "run"
	RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
	# ...and turn them into do-nothing targets
	$(eval $(RUN_ARGS):;@:)
endif

run:
	./$(OUTPUT_FILE) $(RUN_ARGS)

clean:
ifneq ($(wildcard $(BIN_DIR)),)
	rm -r $(BIN_DIR)
endif

rebuild:
	+make clean
	+make build

.PHONY: all build run clean rebuild
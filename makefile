# Compiler
CXX = g++
CFLAGS = $(MISC_FLAGS) $(INCLUDES) $(DEBUG_FLAGS)
MISC_FLAGS = -pthread -std=c++0x -O2
DEBUG_FLAGS = -ggdb
VPATH = $(SRC_DIR):$(TEST_DIR)

O_DIR = bin

CONFIG_DIR = config
SRC_DIR = src
TEST_DIR = tests

INCLUDES = $(INCLUDE_CONFIG) $(INCLUDE_DIRS) $(INCLUDE_LIBS)
INCLUDE_CONFIG = -I$(CONFIG_DIR)


OBJS := $(patsubst %.h,%.o,$(wildcard $(SRC_DIR)/*.h))

tests = $(wildcard $(TEST_DIR)/*_test.cpp)

# TODO(kent): restructure 3rdparty items to avoid filename collision
THIRD_PARTY_DIRS = $(wildcard 3rdparty/*/latest/)

INCLUDE_LIBS = $(foreach INCLUDEDIR, $(THIRD_PARTY_DIRS), -I$(INCLUDEDIR))
INCLUDE_DIRS = -I$(SRC_DIR)

.PHONY : tests
tests: $(O_DIR)/run_tests

$(O_DIR)/run_tests: run_tests.cpp $(tests)
	$(CXX) $(CFLAGS) $< -o $@


.PHONY : raytracer
raytracer: $(O_DIR)/raytracer

$(O_DIR)/raytracer: main.cpp $(SRC_DIR)/*.cpp $(SRC_DIR)/*.h
	$(CXX) $(CFLAGS) $< -o $@

.PHONY : clean
clean:
	rm -rf $(O_DIR)/* $(SRC_DIR)/*.o

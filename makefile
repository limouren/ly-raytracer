# Compiler
CXX = g++
CFLAGS = $(includes) $(DEBUG_FLAGS)
DEBUG_FLAGS = -ggdb
VPATH = $(SRC_DIR):$(TEST_DIR)

O_DIR = bin

CONFIG_DIR = config
SRC_DIR = src
TEST_DIR = tests

includes = $(include_config) $(include_libs) $(include_dirs)
include_config = -I$(CONFIG_DIR)


OBJS := $(patsubst %.h,%.o,$(wildcard $(SRC_DIR)/*.h))

tests = $(wildcard $(TEST_DIR)/*_test.cpp)

THIRD_PARTY_DIRS = $(wildcard 3rdparty/*/latest/)

include_libs = $(foreach includedir,$(THIRD_PARTY_DIRS), -I$(includedir))
include_dirs = -I$(SRC_DIR)

.PHONY : tests
tests: $(O_DIR)/run_tests

$(O_DIR)/run_tests: run_tests.cpp $(tests)
	$(CXX) $(CFLAGS) $< -o $@


.PHONY : main
main: $(O_DIR)/main

$(O_DIR)/main: main.cpp $(SRC_DIR)/*.cpp $(SRC_DIR)/*.h
	$(CXX) $(CFLAGS) $< -o $@

.PHONY : clean
clean:
	rm -rf $(O_DIR)/* $(SRC_DIR)/*.o

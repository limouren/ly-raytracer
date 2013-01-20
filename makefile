# Compiler
CXX = g++
CFLAGS = $(includes)
VPATH = $(SRC_DIR):$(TEST_DIR)

O_DIR = bin

CONFIG_DIR = config
SRC_DIR = src
TEST_DIR = tests

includes = $(include_config) $(include_libs) $(include_dirs)
include_config = -I$(CONFIG_DIR)

bitmap_dir = 3rdparty/bitmap/

# C++ Boost
boost_dir = 3rdparty/boost/latest/

OBJS := $(patsubst %.cpp,%.o,$(wildcard $(SRC_DIR)/*.cpp))


.PHONY : %(SRC_DIR)/%.h
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CXX) $(CFLAGS) -c $< -o $@

.PHONY : all
all: $(objects)


tests = $(wildcard $(TEST_DIR)/*_test.cpp)

include_libs = -I$(boost_dir) -I$(bitmap_dir)
include_dirs = -I$(SRC_DIR)

.PHONY : tests
tests: $(O_DIR)/run_tests

$(O_DIR)/run_tests: run_tests.cpp $(tests)
	$(CXX) $(CFLAGS) $< -o $@


.PHONY : main
main: $(O_DIR)/main

$(O_DIR)/main: main.cpp $(OBJS)
	$(CXX) $(CFLAGS) $< -o $@

.PHONY : clean
clean:
	rm -rf $(O_DIR)/*

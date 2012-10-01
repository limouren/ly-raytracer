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


# C++ Boost
boost_dir = 3rdparty/boost/latest/

objects = $(patsubst %.h,%.o,$(wildcard core/*.h))


.PHONY : all
all: $(objects)


tests = $(wildcard $(TEST_DIR)/*_test.cpp)

include_libs = -I$(boost_dir)
include_dirs = -I$(SRC_DIR)

.PHONY : tests
tests: $(O_DIR)/run_tests

$(O_DIR)/run_tests: run_tests.cpp $(tests)
	$(CXX) $(CFLAGS) -o $@ $<


.PHONY : clean
clean:
	rm -rf $(O_DIR)/*

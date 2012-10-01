# Compiler
CXX = g++
CFLAGS = $(includes)
VPATH = core:tests

O_DIR = bin

CONFIG_DIR = config
CORE_DIR = core
TEST_DIR = tests

includes = $(include_config) $(include_libs) $(include_dirs)
include_config = -I$(CONFIG_DIR)



# C++ Boost
boost_dir = 3rdparty/boost/latest/


objects = $(patsubst %.h,%.o,$(wildcard core/*.h))


.PHONY : all
all: $(objects)


tests = $(wildcard $(TEST_DIR)/test_*.cpp)
test_objs = $(patsubst $(TEST_DIR),$(O_DIR),(patsubst test_%.cpp,test_%.o,$(tests)))

include_libs = -L$(boost_dir)
include_dirs = -I$(CORE_DIR)

.PHONY : tests
tests: $(O_DIR)/run_tests

$(O_DIR)/run_tests: run_tests.cpp $(tests)
	$(CXX) $(CFLAGS) -o $@ $<


.PHONY : clean
clean:
	rm -rf $(O_DIR)/*

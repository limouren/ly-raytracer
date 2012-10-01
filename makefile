# Compiler
CXX = g++
CFLAGS = $(include_dirs)
VPATH = core:tests

O_DIR = bin
TEST_DIR = tests

include_dirs = -L$(boost_dir)


# C++ Boost
boost_dir = 3rdparty/boost/latest/


objects = $(patsubst %.h,%.o,$(wildcard core/*.h))
tests = $(patsubst test_%.cpp,test_%.o,$(wildcard test_*.cpp))


.PHONY : all
all: $(objects)


.PHONY : tests
tests: $(O_DIR)/run_tests


$(O_DIR)/run_tests: run_tests.cpp $(tests)
	$(CXX) $(CFLAGS) -o $@ $<

$

.PHONY : clean
clean:
	rm -rf $(O_DIR)/*

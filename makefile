# Compiler
CXX = g++
CFLAGS = $(MISC_FLAGS) $(INCLUDES) $(DEBUG_FLAGS)
MISC_FLAGS = -pthread -std=c++0x -O3
DEBUG_FLAGS = -ggdb
VPATH = $(SRC_DIR):$(TEST_DIR)

O_DIR = bin

CONFIG_DIR = config
SRC_DIR = src
THIRD_PARTY_DIR = 3rdparty

INCLUDES = $(INCLUDE_CONFIG) $(INCLUDE_SRC) $(THIRD_PARTY_LIBS)
INCLUDE_CONFIG = -I$(CONFIG_DIR)
INCLUDE_SRC = -I$(SRC_DIR)
THIRD_PARTY_LIBS = -I$(THIRD_PARTY_DIR)

OBJS := $(patsubst %.h,%.o,$(wildcard $(SRC_DIR)/*.h))

.PHONY : raytracer
raytracer: $(O_DIR)/raytracer

$(O_DIR)/raytracer: main.cpp $(SRC_DIR)/*.cpp $(SRC_DIR)/*.h
	$(CXX) $(CFLAGS) $< -o $@

.PHONY : clean
clean:
	rm -rf $(O_DIR)/* $(SRC_DIR)/*.o

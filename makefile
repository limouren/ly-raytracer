# Compiler
CXX = g++
CFLAGS = $(MISC_FLAGS) $(INCLUDES) $(DEBUG_FLAGS)
MISC_FLAGS = -pthread -O3
DEBUG_FLAGS = -ggdb
VPATH = $(SRC_DIR):$(TEST_DIR)

O_DIR = bin

CONFIG_DIR = config
SRC_DIR = src
THIRD_PARTY_DIR = 3rdparty

INCLUDES = $(INCLUDE_CONFIG) $(INCLUDE_SRC) $(THIRD_PARTY_LIBS)
INCLUDE_CONFIG = -I$(CONFIG_DIR)
INCLUDE_SRC = -I$(SRC_DIR)
THIRD_PARTY_LIBS = -I$(THIRD_PARTY_DIR) $(WXWIDGET_FLAGS)

OBJS := $(patsubst %.h,%.o,$(wildcard $(SRC_DIR)/*.h))

WXWIDGET_FLAGS = $(shell wx-config --libs --cxxflags)

.PHONY : raytracer
raytracer: $(O_DIR)/raytracer

$(O_DIR)/raytracer: main.cpp $(SRC_DIR)/*.cpp $(SRC_DIR)/*.h
	$(CXX) $< $(CFLAGS) -o $@

.PHONY : clean
clean:
	rm -rf $(O_DIR)/* $(SRC_DIR)/*.o

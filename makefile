# Compiler
CXX = g++
CFLAGS = $(MISC_FLAGS) $(INCLUDES) $(DEBUG_FLAGS)
MISC_FLAGS = -pthread -O3
DEBUG_FLAGS = -ggdb
VPATH = $(SRC_DIR):$(TEST_DIR)

BIN_DIR = bin

CONFIG_DIR = config
GUI_DIR = gui
RAYTRACER_DIR = raytracer
THIRD_PARTY_DIR = 3rdparty

INCLUDES = -I$(CONFIG_DIR) -I$(GUI_DIR) -I$(RAYTRACER_DIR)\
           -I$(THIRD_PARTY_DIR) $(WXWIDGET_FLAGS)

OBJS := $(patsubst %.h,%.o,$(wildcard $(SRC_DIR)/*.h))

WXWIDGET_FLAGS = $(shell wx-config --libs --cxxflags)

.PHONY : raytracer
raytracer: $(BIN_DIR)/raytracer

$(BIN_DIR)/raytracer: main.cpp $(GUI_DIR)/*.cpp $(GUI_DIR)/*.h\
                    $(RAYTRACER_DIR)/*.cpp $(RAYTRACER_DIR)/*.h
	$(CXX) $< $(CFLAGS) -o $@

.PHONY : clean
clean:
	rm -rf $(BIN_DIR)/*

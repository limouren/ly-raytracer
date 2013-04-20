# Compiler
CXX = g++
CFLAGS = $(MISC_FLAGS) $(INCLUDES) $(DEBUG_FLAGS)
MISC_FLAGS = -pthread -O3
DEBUG_FLAGS = -ggdb
VPATH = $(SRC_DIR):$(TEST_DIR)

O_DIR = bin

CONFIG_DIR = config
GUI_DIR = gui
RAYTRACER_DIR = raytracer
THIRD_PARTY_DIR = 3rdparty

INCLUDES = -I$(CONFIG_DIR) -I$(GUI_DIR) -I$(RAYTRACER_DIR)\
           -I$(THIRD_PARTY_DIR) $(WXWIDGET_FLAGS)

OBJS := $(patsubst %.h,%.o,$(wildcard $(SRC_DIR)/*.h))

WXWIDGET_FLAGS = $(shell wx-config --libs --cxxflags)

.PHONY : raytracer
raytracer: $(O_DIR)/raytracer

$(O_DIR)/raytracer: main.cpp $(GUI_DIR)/*.cpp $(GUI_DIR)/*.h\
                    $(RAYTRACER_DIR)/*.cpp $(RAYTRACER_DIR)/*.h
	$(CXX) $< $(CFLAGS) -o $@

.PHONY : clean
clean:
	rm -rf $(O_DIR)/* $(SRC_DIR)/*.o

CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 -std=c++0x -Wno-deprecated-declarations

OBJS =		lights_test.o 

MAIN_DIR = ../lights
MAIN_SRC += $(wildcard $(MAIN_DIR)/*.cpp) 
MAIN_SRC :=$(filter-out ../lights/lights.cpp,$(MAIN_SRC))
$(info $(MAIN_SRC))
OBJS += $(MAIN_SRC:.cpp=.o)
CXXFLAGS += -I $(MAIN_DIR)

STUBS_DIR = ./stubs
STUBS_SRC += $(wildcard $(STUBS_DIR)/*.cpp) 
OBJS += $(STUBS_SRC:.cpp=.o)
CXXFLAGS += -I $(STUBS_DIR)

CONTROLLINO_DIR=/home/fraco/.arduinocdt/libraries/CONTROLLINO_Library/
CXXFLAGS += -I $(CONTROLLINO_DIR)
CXXFLAGS += -DCONTROLLINO_MAXI -DARDUINO=10608


CXXFLAGS += `pkg-config libcpptest --libs --cflags`

LIBS =

TARGET =	lights_test

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS) $(CXXFLAGS) 

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

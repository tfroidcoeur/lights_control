CXXFLAGS =	-O0 -g -Wall -fmessage-length=0 -std=c++0x -Wno-deprecated-declarations

SRCS = lights_test.cpp

MAIN_DIR = ../lights
MAIN_SRC += $(wildcard $(MAIN_DIR)/*.cpp)
MAIN_SRC :=$(filter-out ../lights/lights.cpp,$(MAIN_SRC))
SRCS += $(MAIN_SRC)
CXXFLAGS += -I $(MAIN_DIR)

STUBS_DIR = ./stubs
STUBS_SRC += $(wildcard $(STUBS_DIR)/*.cpp)
SRCS += $(STUBS_SRC)
CXXFLAGS += -I $(STUBS_DIR)

CONTROLLINO_DIR=/home/fraco/.arduinocdt/libraries/CONTROLLINO_Library/
CXXFLAGS += -I $(CONTROLLINO_DIR)
CXXFLAGS += -DCONTROLLINO_MAXI -DARDUINO=10608

CXXFLAGS += `pkg-config libcpptest --libs --cflags`

OBJS := $(SRCS:.cpp=.o)

LIBS =

TARGET =	lights_test

all: depend $(TARGET)
depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CFLAGS) $(CXXFLAGS) -MM $^ -MF  ./.depend;


$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS) $(CXXFLAGS)


clean:
	rm -f $(OBJS) $(TARGET)

test: all
	./$(TARGET)

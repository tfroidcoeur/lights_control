MAIN_DIR = ../src
LIB_DIR=../lib
CONTROLLINO_DIR=../lib/controllino/
SIGSLOT_LIB=$(LIB_DIR)/sigslot

CXXFLAGS =	-O0 -g -Wall -fmessage-length=0 -std=c++0x -Wno-deprecated-declarations

SRCS = $(wildcard *.cpp)

MAIN_SRC += $(wildcard $(MAIN_DIR)/*.cpp)
MAIN_SRC :=$(filter-out $(MAIN_DIR)/lights.cpp,$(MAIN_SRC))
SRCS += $(MAIN_SRC)
CXXFLAGS += -I $(MAIN_DIR)

STUBS_DIR = ./stubs
STUBS_SRC += $(wildcard $(STUBS_DIR)/*.cpp)
SRCS += $(STUBS_SRC)
CXXFLAGS += -I $(STUBS_DIR)

CONTROLLINO_FLAVOR?=MAXI
CXXFLAGS += -I $(CONTROLLINO_DIR)
CXXFLAGS += -DCONTROLLINO_$(CONTROLLINO_FLAVOR) -DARDUINO=10608
CXXFLAGS += -DSIGSLOT_PURE_ISO
CXXFLAGS += -DVERSION=\"TEST\"

CXXFLAGS += `pkg-config libcpptest --libs --cflags`

CXXFLAGS += -I $(SIGSLOT_LIB)

OBJS := $(SRCS:.cpp=.o)

LIBS =

TARGET =	lights_test

all: $(TARGET)
depends:= $(patsubst %.o,%.d,$(OBJS))

%.d: %.cpp
	$(CXX) $(CFLAGS) $(CXXFLAGS) -MM $^ -MF  $@;


$(TARGET): $(OBJS) $(depends)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS) $(CXXFLAGS)


clean:
	rm -f $(OBJS) $(depends) $(TARGET)

test: all
	./$(TARGET)

-include $(depends)

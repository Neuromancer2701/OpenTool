CLANG_LEVEL := ../..
LIBRARYNAME = OpenTool

LINK_LIBS_IN_SHARED = 0
SHARED_LIBRARY = 1

CXX_PP  := g++ #clang++
CXX_CC  := gcc #clang
CXX_AR  := ld

#----------------------------
# Common Flags for compile.

GLOBAL_INC =  -I/usr/include
GLOBAL_INC += -I/usr/include/c++/4.8
GLOBAL_INC += -I/usr/include/c++/4.8/backward
GLOBAL_INC += -I/usr/include/x86_64-linux-gnu
GLOBAL_INC += -I/usr/include/x86_64-linux-gnu/c++/4.8
GLOBAL_INC += -I/usr/lib/gcc/x86_64-linux-gnu/4.8/include
GLOBAL_INC += -I/usr/lib/gcc/x86_64-linux-gnu/4.8/include-fixed
GLOBAL_INC += -I/usr/local/include

#----------------------------
# Common C++ compile with flags 
# and application include paths.

CXX_APP := $(CXX_PP)

#----------------------------
# Common linker with flags 
# for application library path.

LINK_APP := $(CXX_PP)

BASE_DIR = .

SRC_DIR    = $(BASE_DIR)/src
SOCKET_DIR = $(BASE_DIR)/lib/sockets
TIMER_DIR  = $(BASE_DIR)/lib/Timer

INC_DIR  := -I$(SRC_DIR)
INC_DIR  += -I$(SOCKET_DIR)
INC_DIR  += -I$(TIMER_DIR)

OBJ_DIR  = $(BASE_DIR)/objects
TARGET_LIB = $(BASE_DIR)/lib

SRCS := $(SRC_DIR)/OpenTool.cpp
SRCS += $(SRC_DIR)/Header.cpp  
SRCS += $(SRC_DIR)/MessageID.cpp 
SRCS += $(SRC_DIR)/Mid0001.cpp 
SRCS += $(SRC_DIR)/Mid0002.cpp 
SRCS += $(SRC_DIR)/Mid0003.cpp 
SRCS += $(SRC_DIR)/Mid0004.cpp 
SRCS += $(SRC_DIR)/Mid0005.cpp 
SRCS += $(SRC_DIR)/Mid9999.cpp
SRCS += $(SRC_DIR)/KeepAliveTimer.cpp
SRCS += $(TIMER_DIR)/Timer.cpp
SRCS += $(SOCKET_DIR)/Client.cpp
SRCS += $(SOCKET_DIR)/Server.cpp

OBJS := $(SRCS:.cpp=.o)

TARGET := $(TARGET_LIB)/libOpenTool.so

LIB := -lrt
###############################################################
# Compiler Flags Additions

CXX_FLAGS = $(INC_DIR) $(GLOBAL_INC) -std=c++0x -Wall -fPIC

###################################################################
# Target groups
# 
.PHONY: libs apps test OpenTool depend clean

all: libs

libs: OpenTool
apps: 
test:

OpenTool: $(TARGET) 

$(TARGET): $(OBJS) Makefile
	   $(LINK_APP) -o $(TARGET) $(OBJS) $(LIB) -shared

###################################################################
.cpp.o:    
	$(CXX_APP) $(CXX_FLAGS) -c -o $@ $< 

###################################################################
clean:
	rm -f $(OBJS)
	rm -f $(TARGET)
	rm -f depend.mk
	rm -f *.bak
		    
###################################################################
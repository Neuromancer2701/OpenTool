CLANG_LEVEL := ../..
LIBRARYNAME = OpenTool

LINK_LIBS_IN_SHARED = 0
SHARED_LIBRARY = 1

CXX_PP  := clang++
CXX_CC  := clang
CXX_AR  := lld

#----------------------------
# Common Flags for compile.

EXTRA_CFLAGS := -Weverything -fPIC 

TARGET_THREADLIB  := $(TARGET_ROOT)/lib/libpthread.so.0
#----------------------------
# Common Flags for link

LD_FLAGS  := -Wl,-rpath,/opt/andrew/lib 
LD_FLAGS  += -Wl,-rpath-link,$(TARGET_LIB) 
LD_FLAGS  += -Wl,-rpath-link,$(TARGET_ROOT)/usr/lib

#----------------------------
# Common C++ compile with flags 
# and application include paths.

CXX_APP := $(CXX_PP)

#----------------------------
# Common linker with flags 
# for application library path.

LINK_APP := $(CXX_PP) $(LD_FLAGS) 
#$(TARGET_THREADLIB)

BASE_DIR = .

INC_DIR  = $(BASE_DIR)
SRC_DIR  = $(BASE_DIR)
OBJ_DIR  = $(BASE_DIR)
TARGET_LIB = $(BASE_DIR)/lib

SRCS := $(SRC_DIR)/HwMonAPI.c
SRCS := $(SRC_DIR)/Header.cpp  
SRCS += $(SRC_DIR)/MessageID.cpp 
SRCS += $(SRC_DIR)/Mid0001.cpp 
SRCS += $(SRC_DIR)/Mid0002.cpp 
SRCS += $(SRC_DIR)/Mid0003.cpp 
SRCS += $(SRC_DIR)/Mid0004.cpp 
SRCS += $(SRC_DIR)/Mid0005.cpp 
SRCS += $(SRC_DIR)/Mid9999.cpp

OBJS := $(SRCS:.c=.o)

TARGET := $(TARGET_LIB)/libOpenTool.so
###############################################################
# Compiler Flags Additions

EXTRA_CFLAGS += -I$(INC_DIR)
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
	   $(LINK_APP) -o $(TARGET) $(OBJS) -shared

###################################################################
.c.o:    
	$(CXX_APP) $(EXTRA_CFLAGS) -c -o $@ $< 

###################################################################
clean:
	rm -f $(OBJS)
	rm -f $(TARGET)
	rm -f depend.mk
	rm -f *.bak
		    
###################################################################
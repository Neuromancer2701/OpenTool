CLANG_LEVEL := ../..
LIBRARYNAME = OpenTool

LINK_LIBS_IN_SHARED = 0
SHARED_LIBRARY = 1

SOURCES := Header.cpp  \
	   MessageID.cpp \
	   Mid0001.cpp \
	   Mid0002.cpp \
	   Mid0003.cpp \
	   Mid0004.cpp \
	   Mid0005.cpp \
	   MessageID.cpp \
	   Mid9999.cpp
 

include $(CLANG_LEVEL)/Makefile

GCC	= gcc
CPP	= g++

RM	= rm -fr

CFLAGS	= -c -Wall -I./include
CFLAGS  += -DCPU_X86
ifeq "$(MAKECMDGOALS)" "debug"
  CFLAGS += -D_DEBUG
endif
ifeq "$(MAKECMDGOALS)" "thread"
  CFLAGS += -D_PTHREAD_
endif

CPPFLAGS	= $(CFLAGS)

LIBS	=     -L/usr/lib 
OBJS	=         /SingleCore/AVLTree.o \
    ./SingleCore/BinTree.o \
    ./SingleCore/BlockList.o \
    ./SingleCore/Cache.o \
    ./SingleCore/CapiAlloc.o \
    ./SingleCore/DeQueue.o \
    ./SingleCore/DoubleList.o \
    ./SingleCore/DSpaceList.o \
    ./SingleCore/Emalloc.o \
    ./SingleCore/GCPtr.o \
    ./SingleCore/Graph.o \
    ./SingleCore/HashAVLTree.o \
    ./SingleCore/HashList.o \
    ./SingleCore/HashRBTree.o \
    ./SingleCore/HashTable.o \
    ./SingleCore/Queue.o \
    ./SingleCore/QuickSort.o \
    ./SingleCore/RBTree.o \
    ./SingleCore/SerialRadixSort.o \
    ./SingleCore/ShortMsg.o \
    ./SingleCore/SingleList.o \
    ./SingleCore/SortTable.o \
    ./SingleCore/SpaceList.o \
    ./SingleCore/SpList.o \
    ./SingleCore/Stack.o \
    ./SingleCore/Tree.o \
    ./SingleCore/Xcopy.o 

CSRCS	=     /SingleCore/AVLTree.c \
    ./SingleCore/BinTree.c \
    ./SingleCore/BlockList.c \
    ./SingleCore/Cache.c \
    ./SingleCore/CapiAlloc.c \
    ./SingleCore/DeQueue.c \
    ./SingleCore/DoubleList.c \
    ./SingleCore/DSpaceList.c 
#    ./SingleCore/Emalloc.c \
#    ./SingleCore/Graph.c \
#    ./SingleCore/HashAVLTree.c \
#    ./SingleCore/HashList.c \
#    ./SingleCore/HashRBTree.c \
#    ./SingleCore/HashTable.c \
#    ./SingleCore/Queue.c \
#    ./SingleCore/QuickSort.c \
#    ./SingleCore/RBTree.c \
#    ./SingleCore/ShortMsg.c \
#    ./SingleCore/SingleList.c \
#    ./SingleCore/SortTable.c \
#    ./SingleCore/SpaceList.c \
#    ./SingleCore/SpList.c \
#    ./SingleCore/Stack.c \
#    ./SingleCore/Tree.c \
#    ./SingleCore/Xcopy.c 

CPPSRCS =     ./SingleCore/GCPtr.cpp \
    ./SingleCore/SerialRadixSort.cpp \

CARGS  =    -Wall -c -I../include

APPS	=   libcapi.a

all: $(APPS)

$(APPS): $(SRCS) Makefile 
	$(GCC) $(CFLAGS) $(CSRCS) $(CPPSRCS)
	ar r libcapi.a  $(OBJS)

clean:
	$(RM) $(APPS) $(OBJS)  *.orig

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
OBJS	=        AVLTree.o \
    BinTree.o \
    BlockList.o \
    Cache.o \
    CapiAlloc.o \
    DeQueue.o \
    DoubleList.o \
    DSpaceList.o \
    Emalloc.o \
    GCPtr.o \
    Graph.o \
    HashAVLTree.o \
    HashList.o \
    HashRBTree.o \
    HashTable.o \
    Queue.o \
    QuickSort.o \
    RBTree.o \
    SerialRadixSort.o \
    ShortMsg.o \
    SingleList.o \
    SortTable.o \
    SpaceList.o \
    SpList.o \
    Stack.o \
    Tree.o 
    

CSRCS	=     ./SingleCore/AVLTree.c \
    ./SingleCore/BinTree.c \
    ./SingleCore/BlockList.c \
    ./SingleCore/Cache.c \
    ./SingleCore/CapiAlloc.c \
    ./SingleCore/DeQueue.c \
    ./SingleCore/DoubleList.c \
    ./SingleCore/DSpaceList.c \
    ./MultiCore/Event.c \
    ./SingleCore/Emalloc.c \
    ./SingleCore/Graph.c \
    ./SingleCore/HashAVLTree.c \
    ./SingleCore/HashList.c \
    ./SingleCore/HashRBTree.c \
    ./SingleCore/HashTable.c \
    ./SingleCore/Queue.c \
    ./SingleCore/QuickSort.c \
    ./SingleCore/RBTree.c \
    ./SingleCore/ShortMsg.c \
    ./SingleCore/SingleList.c \
    ./SingleCore/SortTable.c \
    ./SingleCore/SpaceList.c \
    ./SingleCore/SpList.c \
    ./SingleCore/Stack.c \
    ./SingleCore/Tree.c 
    

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

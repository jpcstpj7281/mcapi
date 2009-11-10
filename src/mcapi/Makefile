GCC	= gcc
CPP	= g++

RM	= rm -fr

CFLAGS	= -c -fopenmp -Wall -I./include -I./dmalloc
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
    Event.o \
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
    Tree.o \
    GCPtr.o \
    SerialRadixSort.o \
    ArrayList.o \
    Atomic.o \
    CapiCommon.o \
    CDHashAVLTree.o \
    CFastRandom.o \
    CLockFreeQueue.o \
    CLockFreeStack.o \
    CMTask.o \
    CNestTaskScheduler.o \
    CRWLock.o \
    CScopedLock.o \
    CSegmentHashTable.o \
    CTaskGraphScheduler.o \
    CTaskTeam.o \
    CThreadPool.o \
    CWaitTaskScheduler.o \
    DHashAVLTree.o \
    Event.o \
    Matrix.o \
    MsgQueue.o \
    MStack.o \
    MTask.o \
    MTList.o \
    MTree.o \
    Parallel_BubbleSort.o \
    Parallel_For.o \
    Parallel_Graph.o \
    Parallel_MergeSort.o \
    Parallel_RadixSort.o \
    Parallel_Search.o \
    ReadWriteLock.o \
	FreeList.o \
	BigMemory.o \
	dmalloc.o \
	MemoryPool.o 

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
    ./MultiCore/ArrayList.c \
    ./MultiCore/Atomic.cpp \
    ./MultiCore/CapiCommon.cpp \
    ./MultiCore/CDHashAVLTree.cpp \
    ./MultiCore/CFastRandom.cpp \
    ./MultiCore/CLockFreeQueue.cpp \
    ./MultiCore/CLockFreeStack.cpp \
    ./MultiCore/CMTask.cpp \
    ./MultiCore/CNestTaskScheduler.cpp \
    ./MultiCore/CRWLock.cpp \
    ./MultiCore/CScopedLock.cpp \
    ./MultiCore/CSegmentHashTable.cpp \
    ./MultiCore/CTaskGraphScheduler.cpp \
    ./MultiCore/CTaskTeam.cpp \
    ./MultiCore/CThreadPool.cpp \
    ./MultiCore/CWaitTaskScheduler.cpp \
    ./MultiCore/DHashAVLTree.cpp \
    ./MultiCore/Event.c \
    ./MultiCore/Matrix.cpp \
    ./MultiCore/MsgQueue.c \
    ./MultiCore/MStack.c \
    ./MultiCore/MTask.c \
    ./MultiCore/MTList.c \
    ./MultiCore/MTree.c \
    ./MultiCore/Parallel_BubbleSort.cpp \
    ./MultiCore/Parallel_For.cpp \
    ./MultiCore/Parallel_Graph.cpp \
    ./MultiCore/Parallel_MergeSort.cpp \
    ./MultiCore/Parallel_RadixSort.cpp \
    ./MultiCore/Parallel_Search.cpp \
    ./MultiCore/ReadWriteLock.c

DMALLOC_SRC = ./dmalloc/FreeList.c \
	./dmalloc/BigMemory.cpp \
	./dmalloc/dmalloc.cpp \
	./dmalloc/MemoryPool.cpp 

CARGS  =    -Wall -c -I../include

APPS	=   libmcapi.a

all: $(APPS)

$(APPS): $(SRCS) Makefile 
	$(GCC) $(CFLAGS) $(CSRCS) $(CPPSRCS) $(DMALLOC_SRC)
	ar r libmcapi.a  $(OBJS)

clean:
	$(RM) $(APPS) $(OBJS)  *.orig

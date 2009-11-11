GCC	= gcc
CPP	= g++

RM	= rm -fr

CFLAGS	= -g -fopenmp -Wall -lstdc++ -I../mcapi/include -I../TestApi
CFLAGS  += -DCPU_X86 -D_TEST
ifeq "$(MAKECMDGOALS)" "debug"
  CFLAGS += -D_DEBUG
endif
ifeq "$(MAKECMDGOALS)" "thread"
  CFLAGS += -D_PTHREAD_
endif

CPPFLAGS	= $(CFLAGS)

LIBS	=     -L/usr/lib -L../mcapi -L../TestApi -lmcapi -ltestapi -lpthread

OBJS	=  TestCapi.o \
    Test_ABS.o \
    Test_AVLTree.o \
    Test_BinTree.o \
    Test_BlockList.o \
    Test_CapiCommon.o \
    Test_CDHashArray.o \
    Test_CDHashAVLTree.o \
    Test_CDistributedQueue.o \
    Test_CLocalQueue.o \
    Test_CMsgQueue.o \
    Test_CNestTaskScheduler.o \
    Test_CQueuePool.o \
    Test_CRankList.o \
    Test_CScopedLock.o \
    Test_CSegmentHashTable.o \
    Test_CStealQueue.o \
    Test_CTaskGraphScheduler.o \
    Test_CTaskScheduler.o \
    Test_DeQueue.o \
    Test_DHashAVLTree.o \
    Test_dmalloc.o \
    Test_DoubleList.o \
    Test_DSpaceList.o \
    Test_Emalloc.o \
    Test_Graph.o \
    Test_HashAVLTree.o \
    Test_HashList.o \
    Test_HashRBTree.o \
    Test_HashTable.o \
    Test_Matrix.o \
    Test_MQuickSort.o \
    Test_MTList.o \
    Test_MTree.o \
    Test_Parallel_For.o \
    Test_Parallel_MergeSort.o \
    Test_Parallel_PrefixSum.o \
    Test_Parallel_RadixSort.o \
    Test_Parallel_Search.o \
    Test_Performance.o \
    Test_Queue.o \
    Test_RBTree.o \
    Test_SerialRadixSort.o \
    Test_SingleList.o \
    Test_SortTable.o \
    Test_SpList.o \
    Test_Stack.o 



CPPSRCS =         TestCapi.cpp \
    Test_ABS.cpp \
    Test_AVLTree.cpp \
    Test_BinTree.cpp \
    Test_BlockList.cpp \
    Test_CapiCommon.cpp \
    Test_CDHashArray.cpp \
    Test_CDHashAVLTree.cpp \
    Test_CDistributedQueue.cpp \
    Test_CLocalQueue.cpp \
    Test_CMsgQueue.cpp \
    Test_CNestTaskScheduler.cpp \
    Test_CQueuePool.cpp \
    Test_CRankList.cpp \
    Test_CScopedLock.cpp \
    Test_CSegmentHashTable.cpp \
    Test_CStealQueue.cpp \
    Test_CTaskGraphScheduler.cpp \
    Test_CTaskScheduler.cpp \
    Test_DeQueue.cpp \
    Test_DHashAVLTree.cpp \
    Test_dmalloc.cpp \
    Test_DoubleList.cpp \
    Test_DSpaceList.cpp \
    Test_Emalloc.cpp \
    Test_Graph.cpp \
    Test_HashAVLTree.cpp \
    Test_HashList.cpp \
    Test_HashRBTree.cpp \
    Test_HashTable.cpp \
    Test_Matrix.cpp \
    Test_MQuickSort.cpp \
    Test_MTList.cpp \
    Test_MTree.cpp \
    Test_Parallel_For.cpp \
    Test_Parallel_MergeSort.cpp \
    Test_Parallel_PrefixSum.cpp \
    Test_Parallel_RadixSort.cpp \
    Test_Parallel_Search.cpp \
    Test_Performance.cpp \
    Test_Queue.cpp \
    Test_RBTree.cpp \
    Test_SerialRadixSort.cpp \
    Test_SingleList.cpp \
    Test_SortTable.cpp \
    Test_SpList.cpp \
    Test_Stack.cpp 




CARGS  =    -Wall -c -I../mcapi/include

APPS	=   test_capi

all: $(APPS)

$(APPS): $(SRCS) Makefile 
	$(GCC) -c $(CFLAGS) $(CPPSRCS)
	$(GCC) -o test_capi $(CFLAGS) $(OBJS) $(LIBS)

clean:
	$(RM) $(APPS) $(OBJS)  *.orig

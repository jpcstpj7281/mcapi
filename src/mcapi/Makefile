GCC	= gcc
CPP	= g++

RM	= rm -fr

CFLAGS	= -c -Wall -I./Include
CFLAGS  += -DCPU_X86
ifeq "$(MAKECMDGOALS)" "debug"
  CFLAGS += -D_DEBUG
endif
ifeq "$(MAKECMDGOALS)" "thread"
  CFLAGS += -D_PTHREAD_
endif

CPPFLAGS	= $(CFLAGS)

LIBS	=     -L/usr/lib 
OBJS	=     BlockList.o DoubleList.o HashTable.o Queue.o QuickSort.o SortTable.o SingleList.o Stack.o 
SRCS	=     ./SingleCore/BlockList.c ./SingleCore/DoubleList.c ./SingleCore/HashTable.c \
              ./SingleCore/Queue.c ./SingleCore/QuickSort.c ./SingleCore/SortTable.c ./SingleCore/SingleList.c ./SingleCore/Stack.c

CARGS  =    -Wall -c -I../Include

APPS	=   libcapi.a

all: $(APPS)

$(APPS): $(SRCS) Makefile 
	$(GCC) $(CFLAGS) $(SRCS) 
	ar r libcapi.a  $(OBJS)

clean:
	$(RM) $(APPS) $(OBJS)  *.orig

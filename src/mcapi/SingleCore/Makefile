GCC	= gcc
CPP	= g++

RM	= rm -fr

CFLAGS	= -c -Wall -I../Include
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
SRCS	=     BlockList.c DoubleList.c HashTable.c Queue.c QuickSort.c SortTable.c SingleList.c Stack.c

CARGS  =    -Wall -c -I../Include

APPS	=   libcapi.a

all: $(APPS)

$(APPS): $(SRCS) Makefile $(OBJS)
	$(GCC) $(CARGS) $(SRCS) 
	ar r libcapi.a  $(OBJS)

clean:
	$(RM) $(APPS) $(OBJS)  *.orig

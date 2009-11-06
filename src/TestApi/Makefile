GCC	= gcc
CPP	= g++

RM	= rm -fr

CFLAGS	= -g -pthread -c -Wall -W -I../capi/Include -L../capi -L/usr/lib  -lcapi -lpthread
CFLAGS  += -DCPU_X86
ifeq "$(MAKECMDGOALS)" "debug"
  CFLAGS += -D_DEBUG
endif
ifeq "$(MAKECMDGOALS)" "thread"
  CFLAGS += -D_PTHREAD_
endif

CPPFLAGS	= $(CFLAGS)

LIBS	=     -L../capi -L/usr/lib  -lcapi -lpthread
OBJS	=     THashTable.o TestCaseMgr.o TestManager.o 
SRCS	=     THashTable.cpp TestCaseMgr.cpp TestManager.cpp


CARGS  =    

APPS	=   libtestapi.a

all: $(APPS)

$(APPS): $(SRCS) Makefile $(OBJS)
	$(GCC) $(CFLAGS) $(SRCS) $(LIBS)
	ar r libtestapi.a  $(OBJS)

clean:
	$(RM) $(APPS) $(OBJS)  *.orig

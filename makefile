# ------------------------------------------------
# Generic Makefile
#
# Author: yanick.rochon@gmail.com
# Date  : 2010-11-05
#
# Changelog :
#   0.01 - first version
# ------------------------------------------------

# project name (generate executable with this name)
CC       = gcc
# compiling flags here
CFLAGS   = -Wall -I.

LINKER   = gcc -o
# linking flags here
LFLAGS   = -Wall -lpthread

# change these to set the proper directories where each files shoould be
SRCDIR   = ./src
OBJDIR   = ./obj
BINDIR   = ./bin
INCDIR   = ./include

all: dir_create queue

queue: queue_obj ringfifo_obj
	$(CC) $(OBJDIR)/queue.o $(OBJDIR)/ringfifo.o $(LFLAGS) -o $(BINDIR)/queue 

ringfifo_obj: $(SRCDIR)/ringfifo.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $(SRCDIR)/ringfifo.c -o $(OBJDIR)/ringfifo.o

queue_obj: $(SRCDIR)/queue.c
	$(CC) -I$(INCDIR) $(CFLAGS) -c $(SRCDIR)/queue.c -o $(OBJDIR)/queue.o

dir_create:
ifeq "$(wildcard $(OBJDIR) )" ""
	mkdir -p $(OBJDIR)
endif
ifeq "$(wildcard $(BINDIR) )" ""
	mkdir -p $(BINDIR)
endif

clean:
	rm -rf $(BINDIR)/queue
	rm -rf $(OBJDIR)/*.*


####################################
# This is a makefile for mawk,
# an implementation of AWK 
####################################
#
#

CC = cc
CFLAGS = -O  -DMAWK -I..

O=rexp.o rexp0.o rexp1.o rexp2.o rexp3.o rexpdb.o wait.o

all : $(O)

$(O) : rexp.h wait.h

clean :
	rm -f *.o 

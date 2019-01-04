CC = gcc
BOOSTLIB = -std=c99 -lm
SRCFILE = DL_bpnet.c

all:
		$(CC) $(SRCFILE) $(BOOSTLIB) -o DL_bpnet
clean:
		rm -rf DL_bpnet

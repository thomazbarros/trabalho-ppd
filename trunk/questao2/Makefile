PVM_ARCH=LINUX
PVM_ROOT = ${HOME}/pvm3
DESTDIR = $(HOME)/pvm3/bin/$(PVM_ARCH)
CFLAGS = -I$(PVM_ROOT)/include
LDFLAGS = -L$(PVM_ROOT)/lib/$(PVM_ARCH)
LIBS = -lpvm3 -lgpvm3

all: seq_rank par_rank1
seq_rank:
	$(CC) -o seq_rank seq_rank.o $(LDFLAGS) $(LIBS) 
par_rank1: par_rank1.o
	$(CC) -o par_rank1 par_rank1.o $(LDFLAGS) $(LIBS)

clean:
clean:
	rm -f seq_rank par_rank1 *.o 
install:
	cp seq_rank $(DESTDIR)
	cp par_rank1 $(DESTDIR)

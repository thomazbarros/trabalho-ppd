PVM_ARCH=LINUX
PVM_ROOT = ${HOME}/pvm3
DESTDIR = $(HOME)/pvm3/bin/$(PVM_ARCH)
CFLAGS = -I$(PVM_ROOT)/include
LDFLAGS = -L$(PVM_ROOT)/lib/$(PVM_ARCH)
LIBS = -lpvm3 -lgpvm3

all: calpipar calpi
calpipar: calpipar.o
	$(CC) -o calpipar calpipar.o $(LDFLAGS) $(LIBS)
par_rank1: calpi
	$(CC) -o calpi calpi.o 

clean:
clean:
	rm -f calpipar calpi *.o 
install:
	cp calpipar $(DESTDIR)
	cp calpi $(DESTDIR)

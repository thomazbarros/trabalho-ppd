PVM_ARCH=LINUX
PVM_ROOT = ${HOME}/pvm3
DESTDIR = $(HOME)/pvm3/bin/$(PVM_ARCH)
CFLAGS = -I$(PVM_ROOT)/include
LDFLAGS = -L$(PVM_ROOT)/lib/$(PVM_ARCH)
LIBS = -lpvm3 -lgpvm3

all: ola outro_ola pvmconf mensagem buffer simples group calpipar par_rank1
ola: ola.o
	$(CC) -o ola ola.o $(LDFLAGS) $(LIBS)
outro_ola: outro_ola.o
	$(CC) -o outro_ola outro_ola.o $(LDFLAGS) $(LIBS)
pvmconf: pvmconf.o
	$(CC) -o pvmconf pvmconf.o $(LDFLAGS) $(LIBS)
mensagem: mensagem.o
	$(CC) -o mensagem mensagem.o $(LDFLAGS) $(LIBS)
buffer: buffer.o
	$(CC) -o buffer buffer.o $(LDFLAGS) $(LIBS)
simples: simples.o
	$(CC) -o simples simples.o $(LDFLAGS) $(LIBS)
group: group.o
	$(CC) -o group group.o $(LDFLAGS) $(LIBS)
calpipar: calpipar.o
	$(CC) -o calpipar calpipar.o $(LDFLAGS) $(LIBS)
par_rank1: par_rank1.o
	$(CC) -o par_rank1 par_rank1.o $(LDFLAGS) $(LIBS)

clean:
clean:
	rm -f ola outro_ola simples mensagem buffer pvmconf calpipar par_rank1 *.o 
install:
	cp outro_ola $(DESTDIR)
	cp ola $(DESTDIR)
	cp mensagem $(DESTDIR)
	cp buffer  $(DESTDIR)
	cp pvmconf  $(DESTDIR)
	cp simples  $(DESTDIR)
	cp group  $(DESTDIR)
	cp calpipar $(DESTDIR)
	cp par_rank1 $(DESTDIR)

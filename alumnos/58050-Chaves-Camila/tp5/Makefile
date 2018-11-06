CFLAGS= -Wall
CC=gcc
LDLIBS= -lpthread
TARGETS=servidor

all: $(TARGETS)

servidor: servidor.c struct_inicializacion.c conexiones_socket.c 
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

clean: 
	rm $(TARGETS) *.o

CFLAGS= -Wall
CC=gcc
TARGETS= cliente servidor

all: $(TARGETS)

servidor: servidor.c struct_inicializacion.c conexiones_socket.c 
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

cliente: cliente.c struct_inicializacion.c  
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

clean: 
	rm $(TARGETS) *.o


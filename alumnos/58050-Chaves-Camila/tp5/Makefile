CFLAGS= -Wall
CC=gcc
LDLIBS= -pthread
TARGETS= servidor

all: $(TARGETS)

servidor: servidor.c struct_inicializacion.c conexiones_socket.c 
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@


clean: 
	rm $(TARGETS) *.o

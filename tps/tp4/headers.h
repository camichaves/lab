int conectarse(char* ip,struct sockaddr_in procrem, int fd);
int bind_socket(int a,struct sockaddr_in procrem, int fd);
int def_socket();
void def_puerto(int puerto, struct sockaddr_in procrem);
void eliminar_hijos_zombies();


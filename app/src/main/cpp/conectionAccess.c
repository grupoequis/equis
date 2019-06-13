#include "para.h"

char conexionError[] = "Error de conexion Intente de nuevo";
char ErrorCredenciales[] = "Error al validar el mail, intente de nuevo";
char confirmado[] = "Confirmado";
char extern mailEnviado[];
int connectSocket(const char *const server,
                 const char *const port){
    struct addrinfo hints;
    struct addrinfo *res0;
    struct addrinfo *res;
    int sock;



    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = 0;
    hints.ai_protocol = IPPROTO_TCP;

    if(getaddrinfo(server, port, &hints, &res0) != 0){
        return -1;
    }

    for(res = res0; res; res = res->ai_next){
        sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if(sock < 0){
            continue;
        }

        if(connect(sock, res->ai_addr, res->ai_addrlen) < 0){
            close(sock);
            sock = -1;
        }
        else{
            break;
        }
    }

    freeaddrinfo(res0);
    if(sock < 0){
        return -1;
    }

    return sock;
}

void CloseConnection(){
    if(open_fd){
        close(open_fd);
        open_fd = -1;
    }
}

int ConnectToServer(char* server_addr,char* port){

    if(open_fd > 0){
        return open_fd;
    }
    open_fd = connectSocket(server_addr,port);
    return open_fd;
}
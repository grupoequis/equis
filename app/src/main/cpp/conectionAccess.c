#include "para.h"

char conexionError[] = "Error de conexion Intente de nuevo";
char ErrorCredenciales[] = "Error al validar el mail, intente de nuevo";
char confirmado[] = "Confirmado";
char extern mailEnviado[];

SSL_CTX* InitCTX(){

    BIO *outbio = NULL;
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    SSL_load_error_strings();

    outbio    = BIO_new(BIO_s_file());
    outbio    = BIO_new_fp(stdout, BIO_NOCLOSE);

    if(SSL_library_init() < 0){
        BIO_printf(outbio, "Could not initialize the OpenSSL library !\n");
    }

    method = SSLv23_client_method();
    ctx = SSL_CTX_new(method);
    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);

    return ctx;
}




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

void CloseConnection(int fd){
    if(fd){
        close(fd);
    }
}

int ConnectToServer(char* server_addr,char* port){

    return connectSocket(server_addr,port);

}
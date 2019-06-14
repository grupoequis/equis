#ifndef PARA_H
#define PARA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>


#define UID "intentodecorreo2@gmail.com"
#define PWD "z1x2c3v4b5n6m7"
#define domain "smtp.gmail.com"


#define smtp_domain_name "smtp.gmail.com"
#define smtp_port  "587"

#define imap_domain_name "imap.gmail.com"
#define imap_port  "993"
#define TRY 10

    char mensajeRecibido[1000];
//char* revisarCorreo();

int enviarMensaje();
int open_fd;
SSL *open_ssl;
int open_imap_fd;
SSL *open_imap_ssl;
char* revisarLista();


char User[100];
char Password[100];
char emisor[1000];

char* IniciarCorreo(const char* mail, const char* password);

char* EnviarCorreo(const char* from, const char* to,const char* subject, const char * message);

void CloseEverything(int fd, SSL* ssl);
char* IniciarImap(const char* mail, const char* password);
void CloseConnection(int fd);
int ConnectToServer(char* server_addr,char* port);
int connectSocket(const char *const server,
                 const char *const port);

char* MailHeader(const char* from, const char* to, const char* subject,
                 const char* mime_type, const char* charset);

char *base64_encode(char* encoded_data,const unsigned char * data,
                    size_t input_length);
void build_decoding_table();
char* esperarRespuesta();
void base64_cleanup();

#ifdef __cplusplus
}
#endif

#endif /* PARA_H */

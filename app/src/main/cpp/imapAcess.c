
#include "para.h"
extern char conexionError[];
extern char ErrorCredenciales[];
extern char confirmado[];
extern char mailEnviado[];
char* IniciarImap(const char* mail, const char* password){

    SSL *ssl;
    int necesario;
    int enviado;
    int connected_fd = 0;
    char buff[1000];
    char estado[1000];

    if(open_imap_fd > 0) {
        CloseConnection(open_imap_fd);
    }
    for (int i = 0; i < TRY && open_imap_fd <= 0; ++i) {
        open_imap_fd = ConnectToServer(imap_domain_name,imap_port);

    }

    if(open_imap_fd <= 0){
        return conexionError;
    }else{
        connected_fd = open_imap_fd;
    }
    SSL_CTX* ctx = InitCTX();
    int recvd = 0;
    char recv_buff[4768];
    int sdsd;

    ssl = SSL_new(ctx);
    int returnsetfd;
    returnsetfd = SSL_set_fd(ssl, connected_fd);
    if(returnsetfd==0){
        return conexionError;
    }
    int retSSL = SSL_connect(ssl);
    for (int i = 0; i < TRY && retSSL <= 0; ++i) {
        retSSL = SSL_connect(ssl);
    }
    if(retSSL <= 0 ){
        retSSL = SSL_get_error(ssl, retSSL);
        close(open_imap_fd);
        open_fd = -1;
        open_imap_fd = -1;
        return conexionError;
    }

    sdsd = SSL_read(ssl,(void*)(recv_buff + recvd), sizeof(recv_buff) - recvd);
    recvd += sdsd;
    sprintf(estado,"%s",recv_buff+recvd-sdsd);

    int dfdf;
    sprintf(buff,"tag login %s %s\r\n",mail,password);
    int len = strlen(buff);
    necesario = strlen(buff);
    enviado = 0;
    while(necesario){
        dfdf = SSL_write(ssl,buff+enviado,necesario);
        if(dfdf == -1){
            return conexionError;
        }
        enviado += dfdf;
        necesario -= dfdf;
    }
    sdsd = SSL_read(ssl,(void*)(recv_buff + recvd), sizeof(recv_buff) - recvd);
    recvd += sdsd;
    sprintf(estado,"%s",recv_buff+recvd-sdsd);
    if(!strstr(estado,"OK")){
        return ErrorCredenciales;
    }
    open_imap_ssl = ssl;
    return confirmado;


}
char ImapServerMessage[10000];
char* esperarRespuesta(){
    if(open_imap_fd <= 0 || open_imap_ssl <= 0){
        return "No connectado";
    }
    chageMailBox("inbox");
    char command[1000];
    strcpy(command,"tag idle\r\n");
    SSL_write(open_imap_ssl,command,strlen(command));
    SSL_read(open_imap_ssl,ImapServerMessage,10000);
    if(!strchr(ImapServerMessage,'+')){
        return "Error idling";
    }
    int n = 0;
    int NN;
    int len;
    while(1){
        SSL_read(open_imap_ssl,ImapServerMessage,10000);
        sscanf(ImapServerMessage,"* %d EXISTS",&NN);
        if(NN>n){
            strcpy(command,"done\r\n");
            SSL_write(open_imap_ssl,command,strlen(command));
            SSL_read(open_imap_ssl,command,1000);
            sprintf(command,"tag fetch %d body[header.fields (subject from)]\r\n",NN);
            SSL_write(open_imap_ssl,command,strlen(command));
            len = SSL_read(open_imap_ssl,ImapServerMessage,10000);
            len = SSL_read(open_imap_ssl,ImapServerMessage,10000);
            ImapServerMessage[len] = 0;
            n = NN;
            return ImapServerMessage;
        }
    }

    //return ImapServerMessage;
}

char* chageMailBox(char* mailBox){
    char command[1000];
    sprintf(command,"tag select %s\r\n",mailBox);
    SSL_write(open_imap_ssl,command,strlen(command));
    SSL_read(open_imap_ssl,ImapServerMessage,1000);
    return ImapServerMessage;
}
char* revisarLista(){
    if(open_imap_ssl <= 0)
        return "No open";
    char command[1000];
    int enviado = 0;
    strcpy(command,"tag list \"\" \"*\"\r\n");
    enviado = SSL_write(open_imap_ssl,command,strlen(command));
    SSL_read(open_imap_ssl,mensajeRecibido,1000);
    return mensajeRecibido;

}
char* EsperarMensajes(){
    char command[1000];
    strcpy(command,"tag fetch %d body[text]\r\n");
    SSL_write(open_imap_ssl,command,strlen(command));
    SSL_read(open_imap_ssl,ImapServerMessage,10000);
}
char* selectMailList(char* list){
    int res;
    char command[1000];
    sprintf(command,"tag \"%s\"\r\n",list);
    if(SSL_write(open_imap_ssl,command,strlen(command)) <= 0 || SSL_read(open_imap_ssl,mensajeRecibido,1000) <= 0 ){
        return conexionError;
    }
    if(!strstr(mensajeRecibido,"OK")){
        return conexionError;
    }
    return mensajeRecibido;

}

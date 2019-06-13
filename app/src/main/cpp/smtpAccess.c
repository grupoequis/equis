
#include "para.h"
extern char conexionError[];
extern char ErrorCredenciales[];
extern char confirmado[];
char mailEnviado[100] = "Se ha enviado el mail";
char* IniciarCorreo(const char* mail, const char* password){

    BIO *outbio = NULL;
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    SSL *ssl;
    int necesario;
    int enviado;
    int connected_fd = 0;
    char buff[1000];
    char estado[1000];

    if(open_fd > 0) {
        CloseConnection();
    }
    for (int i = 0; i < TRY && open_fd <= 0; ++i) {
        ConnectToServer(smtp_domain_name,smtp_port);

    }

    if(open_fd <= 0){
        return conexionError;
    }else{
         connected_fd = open_fd;
    }

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

    int recvd = 0;
    char recv_buff[4768];
    int sdsd;
    sdsd = recv(connected_fd,(void*)(recv_buff + recvd), sizeof(recv_buff) - recvd,0);
    if(sdsd == -1){
        return conexionError;
    }
    if(!strstr(recv_buff,"220")){
        return conexionError;
    }
    recvd += sdsd;
    strcpy(buff,"EHLO ");
    strcat(buff,domain);
    strcat(buff,"\r\n");
    send(connected_fd,buff,strlen(buff),0);
    sdsd = recv(connected_fd,(void*)(recv_buff + recvd),sizeof(recv_buff) - recvd,0);
    recvd += sdsd;
    sprintf(estado,"%s",recv_buff);
    if(!strstr(estado,"STARTTLS")){
        return conexionError;
    }
    strcpy(buff,"STARTTLS\r\n");
    send(connected_fd,buff,strlen(buff),0);
    sdsd = recv(connected_fd,(void*)(recv_buff + recvd),sizeof(recv_buff) - recvd,0);
    recvd += sdsd;
    sprintf(estado,"%s\n",recv_buff+recvd-sdsd);
    if(!strstr(estado,"220")){
        return conexionError;
    }
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
        close(open_fd);
        open_fd = -1;
        return conexionError;
    }

    int dfdf;
    strcpy(buff,"AUTH LOGIN\r\n");
    int len = strlen(buff);
    printf("enviando %s\n",buff);
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

    if(!strstr(estado,"334")){
        return conexionError;
    }

    const unsigned char * uid = (const unsigned char *)mail;
    base64_encode(buff,uid,strlen(mail));
    strcat(buff,"\r\n");
    printf("enviando %s\n",buff);
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
    sdsd = SSL_read(ssl, (void *) (recv_buff + recvd), sizeof(recv_buff) - recvd);
    recvd += sdsd;
    sprintf(estado, "%s\n", recv_buff + recvd - sdsd);
    if(!strstr(estado,"334")){
        return conexionError;
    }


    //printf("Ingrese el password:");
    //scanf("%s\n",_cmd4);
    //base64_encode(_cmd4,PWD,strlen(PWD));

    const unsigned char * pwd = (const unsigned char *)password;
    base64_encode(buff,pwd,strlen(password));
    strcat(buff, "\r\n");
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

    sdsd = SSL_read(ssl, (void *) (recv_buff + recvd), sizeof(recv_buff) - recvd);
    recvd += sdsd;
    sprintf(estado, "recibido: %s\n", recv_buff + recvd - sdsd);
    if(strstr(estado,"Accepted")){
        open_ssl = ssl;
        strcpy(emisor,mail);
        strcpy(User,mail);
        strcpy(Password,password);
        return confirmado;
    }else{
        return ErrorCredenciales;
    }



}



char* EnviarCorreo(const char* from, const char* to,const char* subject, const char * message){
    int necesario;
    int enviado;
    int dfdf;
    int sdsd;
    int recvd;
    char recv_buff[10000];
    char estado[1000];

    SSL *ssl = open_ssl;

    for (int i = 0; i < TRY && !open_ssl; ++i) {
        IniciarCorreo(User,Password);
    }
    if(!open_ssl)
        return conexionError;

    char *header = MailHeader(from,to,subject,"text/plain","US-ASCII");

    char command[1000];
    //strcpy(command,"MAIL FROM: ");
    sprintf(command,"%s<%s>\r\n","MAIL FROM: ",from);
    //strcat(command,"\r\n");
    printf("enviando %s\n",command);
    necesario = strlen(command);
    enviado = 0;
    while(necesario){
        dfdf = SSL_write(ssl,command+enviado,necesario);
        if(dfdf == -1){
            return conexionError;
        }
        enviado += dfdf;
        necesario -= dfdf;
    }

    sdsd = SSL_read(ssl, (void *) (recv_buff + recvd), sizeof(recv_buff) - recvd);
    recvd += sdsd;
    sprintf(estado, "%s\n", recv_buff + recvd - sdsd);

    sprintf(command,"%s<%s>\r\n","RCPT TO: ",to);
    printf("%s\n",command);
    necesario = strlen(command);
    enviado = 0;
    while(necesario){
        dfdf = SSL_write(ssl,command+enviado,necesario);
        if(dfdf == -1){
            return conexionError;
        }
        enviado += dfdf;
        necesario -= dfdf;
    }
    sdsd = SSL_read(ssl, (void *) (recv_buff + recvd), sizeof(recv_buff) - recvd);
    recvd += sdsd;
    sprintf(estado, "%s\n", recv_buff + recvd - sdsd);

    strcpy(command,"DATA\r\n");
    printf("%s\n",command);
    necesario = strlen(command);
    enviado = 0;
    while(necesario){
        dfdf = SSL_write(ssl,command+enviado,necesario);
        if(dfdf == -1){
            return conexionError;
        }
        enviado += dfdf;
        necesario -= dfdf;
    }
    sdsd = SSL_read(ssl, (void *) (recv_buff + recvd), sizeof(recv_buff) - recvd);
    recvd += sdsd;
    sprintf(estado, "%s\n", recv_buff + recvd - sdsd);

    necesario = strlen(header);
    enviado = 0;
    while(necesario){
        dfdf = SSL_write(ssl,header+enviado,necesario);
        if(dfdf == -1){
            return conexionError;
        }
        enviado += dfdf;
        necesario -= dfdf;
    }

    //strcpy(command,"prueba de envio de mail de android\r\n");
    sprintf(command,"%s\r\n",message);
    //strcpy(_cmd8,"chamo pero tu si eres marico");
    printf("enviando %s\n",command);
    necesario = strlen(command);
    enviado = 0;
    while(necesario){
        dfdf = SSL_write(ssl,command+enviado,necesario);
        if(dfdf == -1){
            return conexionError;
        }
        enviado += dfdf;
        necesario -= dfdf;
    }


    strcpy(command,"\r\n.\r\n\r\n");
    printf("enviando %s\n",command);
    necesario = strlen(command);
    enviado = 0;
    while(necesario){
        dfdf = SSL_write(ssl,command+enviado,necesario);
        if(dfdf == -1){
            return conexionError;
        }
        enviado += dfdf;
        necesario -= dfdf;
    }
    sdsd = SSL_read(ssl, (void *) (recv_buff + recvd), sizeof(recv_buff) - recvd);
    recvd += sdsd;
    sprintf(estado, "%s\n", recv_buff + recvd - sdsd);
    return mailEnviado;
}

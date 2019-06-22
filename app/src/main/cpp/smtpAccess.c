
#include <fcntl.h>
#include "para.h"
extern char conexionError[];
extern char ErrorCredenciales[];
extern char confirmado[];
char mailEnviado[100] = "Se ha enviado el mail";
char* IniciarCorreo(const char* mail, const char* password){

    SSL_CTX *ctx;
    SSL *ssl;
    int necesario;
    int enviado;
    int connected_fd = 0;
    char buff[1000];
    char estado[1000];

    if(open_smtp_fd > 0) {
        CloseConnection(open_smtp_fd);
        open_smtp_fd = -1;
    }
    for (int i = 0; i < TRY && open_smtp_fd <= 0; ++i) {
        open_smtp_fd =  ConnectToServer(smtp_domain_name,smtp_port);

    }

    if(open_smtp_fd <= 0){
        return conexionError;
    }else{
         connected_fd = open_smtp_fd;
    }

    ctx = InitCTX();
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
        close(open_smtp_fd);
        open_smtp_fd = -1;
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
        open_smtp_ssl = ssl;
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
    int recvd = 0;
    char recv_buff[10000];
    char estado[1000];

    SSL *ssl = open_smtp_ssl;
    open_smtp_ssl = 0;
    IniciarCorreo(User,Password);
    for (int i = 0; i < TRY && !open_smtp_ssl; ++i) {
        IniciarCorreo(User,Password);
    }
    ssl = open_smtp_ssl;
    if(open_smtp_ssl <= 0)
        return conexionError;

    char *header = MailHeader(from,to,subject,"multipart/mixed;","US-ASCII");
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
    if(sdsd <= 0){
        open_smtp_fd = 0;
        open_smtp_ssl = 0;
        return conexionError;
    }
    recvd += sdsd;
    //se daña aqui
    sprintf(estado, "%s\n", recv_buff + recvd - sdsd);

    while(getRCPT()) {
        sprintf(command, "%s<%s>\r\n", "RCPT TO: ", rcptmail);
        necesario = strlen(command);
        enviado = 0;
        while (necesario) {
            dfdf = SSL_write(ssl, command + enviado, necesario);
            if (dfdf == -1) {
                return conexionError;
            }
            enviado += dfdf;
            necesario -= dfdf;
        }
        sdsd = SSL_read(ssl, (void *) (recv_buff + recvd), sizeof(recv_buff) - recvd);
        recvd += sdsd;
        sprintf(estado, "%s\n", recv_buff + recvd - sdsd);
    }
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
    //char filebase64[4 * ((MAXBUFFERSIZE + 2) / 3)];
    //char filebuffer[1025];
    char* name;
    while(getFileName()){
        strcpy(command,"--unique-boundary-1\r\n");
        sprintf(command+strlen(command),"Content-Type: %s; name=\"%s\"\r\n",mimeType,filename);
        sprintf(command+strlen(command),"Content-Disposition: attachment; filename=\"%s\"\r\n",filename);
        strcat(command,"Content-Transfer-Encoding: base64\r\n");
        sprintf(estado,"enviando %s\n",command);
        SSL_write(ssl,command,strlen(command));
        strcpy(bufferMensaje,filepath);
        int musicfd = open(filepath,O_RDONLY);
        int bytesRead;
        bytesRead = read(musicfd,bufferMensaje,MAXBUFFERSIZE);
        while(bytesRead){
            base64_encode(filebase64,bufferMensaje,bytesRead);
            SSL_write(ssl,filebase64,strlen(filebase64));
            bytesRead = read(musicfd,filename,MAXBUFFERSIZE);
        }
        strcpy(command,"\r\n");
        SSL_write(ssl,command,strlen(command));
        close(musicfd);

    }
    strcpy(command,"--unique-boundary-1\r\n");
    sprintf(command+strlen(command),"Content-Type: text/plain;charset=\"UTF-8\"\r\n");
    sprintf(command+strlen(command),"%s\r\n",message);
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

#include <jni.h>
#include <string>
#include <queue>
#include <utility>
#include <string>
#include "para.h"

class androidFile{
public:
    std::string filename;
    std::string path;
    std::string mimetype;
    androidFile(std::string name,std::string path,std::string mime){
        filename = name;
        this->path = path;
        mimetype = mime;
    }


};
std::queue<androidFile> files;
//std::queue<std::pair<std::string,std::string> > files;
//std::queue<std::string> mimes;
extern char conexionError[];
char* IniciarCorreo(const char* mail,const char* password);

/*
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_smtpclient_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject  this ) {
    std::string hello;
    if(enviarMensaje() == 1){
        hello = "Mensaje enviado";
    } else
        hello = "Error intente de nuevo";
    return env->NewStringUTF(hello.c_str());
}
*/
/*
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_smtpclient_MainActivity_enviarMensaje(
        JNIEnv *env,
        jobject  this ) {
    std::string resultado;
    if(enviarMensaje() == 1){
        resultado = "Mensaje enviado";
    } else
        resultado = "Error intente de nuevo";
    return env->NewStringUTF(resultado.c_str());
}
*/
/*
extern "C" JNIEXPORT jint JNICALL
Java_com_example_smtpclient_MainActivity_ConnectarSocket(
        JNIEnv *env,
        jobject , jstring port, jstring ip) {
    int resultado;
    resultado = ConnectToServer((const char *const)port,(const char *const)ip);
    return resultado;
}
*/
extern "C" JNIEXPORT void JNICALL
Java_com_example_smtpclient_SendMailActivity_AddFile(
        JNIEnv *env,
        jobject /* this */, jstring filename,jstring path, jstring mimeType) {
    files.push(androidFile(env->GetStringUTFChars(filename,0),env->GetStringUTFChars(path,0),env->GetStringUTFChars(mimeType,0) ) );
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_smtpclient_MainActivity_AutenticarCorreo(
        JNIEnv *env,
        jobject /* this */, jstring mail, jstring password) {
    std::string resultado;
    bool b = true;
    resultado = IniciarCorreo(env->GetStringUTFChars(mail,0), env->GetStringUTFChars(password,0));
    for (int i = 0; resultado == conexionError && i < 10; ++i) {
        resultado = IniciarCorreo(env->GetStringUTFChars(mail,0), env->GetStringUTFChars(password,0));
    }
    return env->NewStringUTF(resultado.c_str());
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_example_smtpclient_SendMailActivity_SendMail(
        JNIEnv *env,
        jobject /* this */, jstring to, jstring subject, jstring message) {
    std::string resultado;
    bool b = true;
    for (int i = 0; i < TRY && !strstr(resultado.c_str(),"enviado") ; ++i) {
        resultado = EnviarCorreo(emisor,env->GetStringUTFChars(to,0), env->GetStringUTFChars(subject,0),env->GetStringUTFChars(message,0));
    }
    return env->NewStringUTF(resultado.c_str());
}

int getFileName(){
    if(!files.size()){
        return 0;
    }
    strcpy(filename,files.front().filename.c_str());
    strcpy(mimeType,files.front().mimetype.c_str());
    strcpy(filepath,files.front().path.c_str());
    files.pop();
    return 1;
}
extern "C" JNIEXPORT void JNICALL
Java_com_example_smtpclient_SendMailActivity_logout(
        JNIEnv *env,
        jobject /* this */) {
    if(open_imap_fd > 0){
        close(open_imap_fd);
        open_imap_fd = -1;
    }
    if(open_smtp_fd > 0){
        close(open_smtp_fd);
        open_smtp_fd = -1;
    }
    
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_smtpclient_MainActivity_listarCorreos(
        JNIEnv *env,
        jobject /* this */) {
    std::string resultado;
    bool b = true;
    resultado = revisarLista();
    return env->NewStringUTF(resultado.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_smtpclient_MainActivity_IniciarImap(
        JNIEnv *env,
        jobject /* this */, jstring mail, jstring password) {
    std::string resultado;
    bool b = true;
    resultado = IniciarImap(env->GetStringUTFChars(mail,0), env->GetStringUTFChars(password,0));
    for (int i = 0; resultado == conexionError && i < 10; ++i) {
        resultado = IniciarImap(env->GetStringUTFChars(mail,0), env->GetStringUTFChars(password,0));
    }
    return env->NewStringUTF(resultado.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_smtpclient_MailNotificationService_EsperarCorreo(
        JNIEnv *env,
        jobject /* this */, jstring mail, jstring password) {
    std::string resultado;
    bool b = true;
    resultado = esperarRespuesta();
    for (int i = 0; resultado == conexionError && i < 10; ++i) {
        resultado = IniciarImap(env->GetStringUTFChars(mail,0), env->GetStringUTFChars(password,0));
    }
    if(strstr(&resultado[0],"NO") ){
        return env->NewStringUTF(resultado.c_str());
    }
    return env->NewStringUTF(resultado.c_str());
}
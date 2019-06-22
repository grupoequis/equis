#include <jni.h>
#include <string>
#include <queue>
#include <utility>
#include <list>
#include <string>
#include <vector>
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

    androidFile(std::string name){
        filename = name;
        this->path = "";
        mimetype = "";
    }

    bool operator ==(const androidFile& af) const{
        if(filename == af.filename)
            return true;
        return false;
    }



};
std::list<androidFile> files;
std::list<std::string> destinos;
//std::queue<std::pair<std::string,std::string> > files;
//std::queue<std::string> mimes;
extern char conexionError[];
char* IniciarCorreo(const char* mail,const char* password);

extern "C" JNIEXPORT void JNICALL
Java_com_example_smtpclient_SendMailActivity_AddRCPT(
        JNIEnv *env,
        jobject /* this */, jstring mail) {
    destinos.push_back(env->GetStringUTFChars(mail,0) );
}

int getRCPT(){
    if(destinos.empty()){
        return 0;
    }
    strcpy(rcptmail,destinos.front().c_str());
    return 1;
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_smtpclient_SendMailActivity_AddFile(
        JNIEnv *env,
        jobject /* this */, jstring filename,jstring path, jstring mimeType) {
    files.push_back(androidFile(env->GetStringUTFChars(filename,0),env->GetStringUTFChars(path,0),env->GetStringUTFChars(mimeType,0) ) );
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_smtpclient_SendMailActivity_RemoveFile(
        JNIEnv *env,
        jobject /* this */, jstring filename) {

    files.remove(androidFile(env->GetStringUTFChars(filename,0)) );
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
bool empty = false;
int getFileName(){
    int size = files.size();
    if(empty){
        return 0;
    }
    if(size == 1)
        empty = true;
    strcpy(filename,files.front().filename.c_str());
    strcpy(mimeType,files.front().mimetype.c_str());
    strcpy(filepath,files.front().path.c_str());
    files.pop_front();
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
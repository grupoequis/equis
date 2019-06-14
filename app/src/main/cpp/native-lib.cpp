#include <jni.h>
#include <string>

#include "para.h"

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
    resultado = EnviarCorreo(emisor,env->GetStringUTFChars(to,0), env->GetStringUTFChars(subject,0),env->GetStringUTFChars(message,0));
    return env->NewStringUTF(resultado.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_smtpclient_SendMailActivity_logout(
        JNIEnv *env,
        jobject /* this */) {
    CloseEverything(open_fd,open_ssl);
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
    resultado = esperarRespuesta();
    return env->NewStringUTF(resultado.c_str());
}
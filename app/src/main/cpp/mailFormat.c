#include <string.h>
#include <stdio.h>
#include <time.h>
static char mbuf[1000];
char* MailHeader(const char* from, const char* to, const char* subject,
                 const char* mime_type, const char* charset){
    time_t now;
    time(&now);
    const char *app_brand = "Codeylog Test APP";
    char* mail_header = mbuf;
    memset(mail_header,0,1000);
    char date_buff[100];
    char Branding[6 + strlen(date_buff) + 2 + 10 + strlen(app_brand) + 1 + 1];
    char Sender[6 + strlen(from) + 1 + 1];
    char Recip[4 + strlen(to) + 1 + 1];
    char Subject[8+1 + strlen(subject) + 1 + 1];
    char mime_data[13+1+3+1+1+13+1+strlen(mime_type)+1+1+8+strlen(charset)+1+1+2];
    strftime(date_buff,(33),"%a , %d %b %Y %H:%M:%S",localtime(&now));
    sprintf(Branding,"DATE: %s\r\nX-Mailer: %s\r\n",date_buff, app_brand);
    sprintf(Sender,"From: %s\r\n",from);
    sprintf(Recip, "To: %s\r\n",to);
    sprintf(Subject,"Subject: %s\r\n", subject);
    //sprintf(mime_data, "MIME-Version: 1.0\r\nContent-type: %s; charset=%s\r\n\r\n",
    //        mime_type, charset);

    int mail_header_length = strlen(Branding) + strlen(Sender) + strlen(Recip) +
                             strlen(Subject) + strlen(mime_data) + 10;

    //mail_header = (char*) malloc(mail_header_length * sizeof(char));
    int len_branding = strlen(Branding);
    int len_Sender = strlen(Sender);
    int len_Recip = strlen(Recip);
    int len_Subject = strlen(Subject);
    int len_mime_data = strlen(mime_data);
    memcpy(&mail_header[0], &Branding, len_branding);
    memcpy(&mail_header[0 + len_branding ], &Sender, len_Sender);
    memcpy(&mail_header[0 + len_branding + len_Sender ], &Recip, len_Recip);
    memcpy(&mail_header[0 + len_branding + len_Sender + len_Recip], &Subject,len_Subject);
    //memcpy(&mail_header[0 + len_branding + len_Sender + len_Recip + len_Subject], &Branding,len_mime_data);
    return mail_header;


}

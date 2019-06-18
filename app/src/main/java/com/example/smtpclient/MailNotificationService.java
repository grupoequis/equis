package com.example.smtpclient;

import android.app.Notification;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.support.annotation.Nullable;
import android.support.v4.app.NotificationCompat;
import android.support.v4.app.NotificationManagerCompat;

import java.util.Scanner;

public class MailNotificationService extends Service {

    public static final String CHANNEL_ID = "personal_notifications";
    private final int NOTIFICATION_ID = 001;
    public static String Titulo;
    public static String Contenido;
    public void onCreate(){
        super.onCreate();
    }
    public int onStartCommand(Intent intent, int flags, int startId){

        backGroundNotifier bgn = new backGroundNotifier();
        bgn.start();
        return START_STICKY;
    }

    class backGroundNotifier extends Thread{
        @Override
        public void run(){
            while(true) {
                String message = "Error";
                while(message.contains("Error"))
                    message = (String) MailNotificationService.EsperarCorreo(MainActivity.getEmail(), MainActivity.getPassword());


                Scanner lineas = new Scanner(message);
                lineas.nextLine();
                lineas.findInLine("<");
                String From = lineas.nextLine();
                lineas.findInLine("Subject: ");
                String Subject = lineas.nextLine();
                Titulo = Subject;

                Intent notificationIntent = new Intent(MailNotificationService.this, MailActivity.class);
                PendingIntent pendingIntent = PendingIntent.getActivity(MailNotificationService.this,
                        0, notificationIntent, 0);

                Notification notification = new NotificationCompat.Builder(MailNotificationService.this, MailNotificationService.CHANNEL_ID)
                        .setContentTitle("Nuevo mensaje DE: " + From)
                        .setContentText(Subject)
                        .setSmallIcon(R.drawable.ic_email)
                        .setContentIntent(pendingIntent)
                        .build();
                NotificationManagerCompat notificationManagerCompat = NotificationManagerCompat.from(MailNotificationService.this);
                notificationManagerCompat.notify(1, notification);
            }

        }

    }

    public native static String EsperarCorreo(String mail,String Password);


    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }
}

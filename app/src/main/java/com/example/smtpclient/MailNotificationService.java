package com.example.smtpclient;

import android.app.Notification;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.support.annotation.Nullable;
import android.support.v4.app.NotificationCompat;

public class MailNotificationService extends Service {

    private final String CHANNEL_ID = "personal_notifications";
    private final int NOTIFICATION_ID = 001;
    public void onCreate(){
        super.onCreate();
    }
    public int onStartCommand(Intent intent, int flags, int startId){
        String message = "";

        message = EsperarCorreo(MainActivity.getEmail(),MainActivity.getPassword());
        Intent notificationIntent = new Intent(this,MailActivity.class);
        PendingIntent pendingIntent = PendingIntent.getActivity(this,
                0,notificationIntent,0);

        Notification notification =  new NotificationCompat.Builder(this,CHANNEL_ID)
                .setContentTitle("Nuevo mensaje")
                .setContentText(message)
                .setSmallIcon(R.drawable.ic_email)
                .setContentIntent(pendingIntent)
                .build();

        return START_STICKY;
    }
    public native String EsperarCorreo(String mail,String Password);


    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }
}

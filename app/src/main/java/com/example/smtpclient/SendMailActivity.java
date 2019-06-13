package com.example.smtpclient;

import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class SendMailActivity extends AppCompatActivity implements View.OnClickListener{

    EditText etMailTo;
    EditText etMailContent;
    EditText etMailSubject;
    Button btMailSend;
    Button btLogout;

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_send_mail);
        etMailContent = findViewById(R.id.etMailContent);
        etMailTo = findViewById(R.id.etMailFrom);
        etMailSubject = findViewById(R.id.etMailSubject);
        btMailSend = findViewById(R.id.btMailSend);
        btMailSend.setOnClickListener(this);
        btLogout = findViewById(R.id.btlogout);
        btLogout.setOnClickListener(this);
    }
    public native String SendMail(String to,String subject,String message);
    public native void logout();


    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.btMailSend:
                String to = etMailTo.getText().toString();
                String message = etMailContent.getText().toString();
                String subject = etMailSubject.getText().toString();
                String resultado = SendMail(to,subject,message);
                Toast.makeText(getApplicationContext(),resultado,Toast.LENGTH_SHORT);
                break;
            case R.id.btlogout:
                logout();
                SharedPreferences sharedPreferences = getSharedPreferences(MainActivity.SHARED_PREFS,MODE_PRIVATE);
                SharedPreferences.Editor editor = sharedPreferences.edit();

                editor.putString(MainActivity.EMAILKEY, "");
                editor.putString(MainActivity.PASSWORDKEY, "");
                editor.apply();

                MainActivity.setEmail("");
                MainActivity.setPassword("");
                finish();
                break;
        }
    }
}

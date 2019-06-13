package com.example.smtpclient;

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
    }
    public native String SendMail(String to,String subject,String message);


    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.btMailSend:
                String to = etMailTo.getText().toString();
                String message = etMailContent.getText().toString();
                String subject = etMailSubject.getText().toString();
                String resultado = SendMail(to,subject,message);
                Toast.makeText(getApplicationContext(),resultado,Toast.LENGTH_SHORT);
        }
    }
}

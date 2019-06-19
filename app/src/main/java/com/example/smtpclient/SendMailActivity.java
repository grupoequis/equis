package com.example.smtpclient;

import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class SendMailActivity extends AppCompatActivity implements View.OnClickListener{

    EditText etMailTo;
    EditText etMailContent;
    EditText etMailSubject;
    TextView tvSend;
    TextView tvCancel;

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_linearwrite_mail);
//        etMailContent = findViewById(R.id.etContent);
//        etMailTo = findViewById(R.id.etMailTo);
//        etMailSubject = findViewById(R.id.etMailSubject);
//        tvSend = findViewById(R.id.tvSendMail);
//        tvSend.setOnClickListener(this);
//        tvCancel = findViewById(R.id.tvCancel);
//        tvCancel.setOnClickListener(this);
    }
    public native String SendMail(String to,String subject,String message);
    public native void logout();


    @Override
    public void onClick(View v) {
        /*
        switch (v.getId()){
            case R.id.tvSendMail:
                String to = etMailTo.getText().toString();
                String message = etMailContent.getText().toString();
                String subject = etMailSubject.getText().toString();
                String resultado = SendMail(to,subject,message);
                Toast.makeText(getApplicationContext(),resultado,Toast.LENGTH_SHORT).show();
                break;
            case R.id.tvCancel:
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
        */
    }

}

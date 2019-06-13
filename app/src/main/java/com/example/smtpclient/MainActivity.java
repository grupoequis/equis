package com.example.smtpclient;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity implements View.OnClickListener{

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
    private Button btInterfas;
    private EditText tvMail, tvPassword;
    private TextView tvMuestra;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        final TextView tv = findViewById(R.id.sample_text);
        tv.setText("presiona el boton");
        tvMail = findViewById(R.id.etMail);
        tvPassword = findViewById(R.id.etPassword);
        btInterfas = findViewById(R.id.btIngresar);
        tvMuestra = findViewById(R.id.sample_text);
        btInterfas.setOnClickListener(this);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String AutenticarCorreo(String mail,String password);



    @Override
    public void onClick(View v) {
        Toast.makeText(getApplicationContext(),"iniciando conexion",Toast.LENGTH_SHORT).show();
        switch (v.getId()){
            case R.id.btIngresar:
                String mail = tvMail.getText().toString();
                String password = tvPassword.getText().toString();

                String respuesta = AutenticarCorreo(mail,password);
                Toast.makeText(getApplicationContext(), respuesta,Toast.LENGTH_LONG).show();
                //tvMuestra.setText(respuesta);
                if(!respuesta.contains("Confirmado"))
                    return;
                Intent i = new Intent(this,SendMailActivity.class);
                startActivity(i);
                break;
        }
    }
}

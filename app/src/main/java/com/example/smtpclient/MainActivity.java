package com.example.smtpclient;

import android.content.Intent;
import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
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
    private CheckBox cbSave;

    public static final String SHARED_PREFS= "sharedPrefs";
    public static final String EMAILKEY = "saved_email";
    public static final String PASSWORDKEY = "saved_password";

    private static String password;
    private static String email;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        String res;
        res = IniciarImap("intentodecorreo2@gmail.com","z1x2c3v4b5n6m7");
        String inbox = "";
        if(res.contains("Confirmado")){
            inbox = listarCorreos();
        }
        Intent i = new Intent(this,MailActivity.class);
        i.putExtra("lista",inbox);
        startActivity(i);
        startService();
        loadData();

        final TextView tv = findViewById(R.id.sample_text);
        tv.setText("presiona el boton");
        tvMail = findViewById(R.id.etMail);
        tvPassword = findViewById(R.id.etPassword);
        btInterfas = findViewById(R.id.btIngresar);
        tvMuestra = findViewById(R.id.sample_text);
        btInterfas.setOnClickListener(this);
        cbSave = findViewById(R.id.cbKeepLogin);
    }

    public static void setPassword(String password) {

        MainActivity.password = password;
    }

    public static void setEmail(String email) {
        MainActivity.email = email;

    }

    public static String getEmail() {
        return MainActivity.email;
    }

    public static String getPassword() {
        return MainActivity.password;
    }

    public native String listarCorreos();

    public native String IniciarImap(String mail,String password);

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String AutenticarCorreo(String mail,String password);

    public void saveData(){
        SharedPreferences sharedPreferences = getSharedPreferences(SHARED_PREFS,MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPreferences.edit();

        editor.putString(EMAILKEY, tvMail.getText().toString());
        editor.putString(PASSWORDKEY, tvPassword.getText().toString());

        editor.apply();
    }
    public void loadData(){
        SharedPreferences sharedPreferences = getSharedPreferences(SHARED_PREFS,MODE_PRIVATE);
        email = sharedPreferences.getString(EMAILKEY,"");
        password = sharedPreferences.getString(PASSWORDKEY,"");
        String resultado;
        if(!email.isEmpty() && !password.isEmpty()){
            resultado = IniciarCorreo(email,password);
            if(resultado.contains("Confirmado")){
                Intent i = new Intent(this,SendMailActivity.class);
                startActivity(i);
            }
        }

    }

    public String IniciarCorreo(String mail,String pw){
        return AutenticarCorreo(mail,pw);
    }
    public void Ingresar(){
        Thread t =  new Thread(){
            public void run(){
                try{
                    String mail = tvMail.getText().toString();
                    String password = tvPassword.getText().toString();

                    String respuesta = IniciarCorreo(mail,password);
                    Toast.makeText(getApplicationContext(), respuesta,Toast.LENGTH_LONG).show();
                    if(!respuesta.contains("Confirmado"))
                        return;
                    saveData();
                    Intent i = new Intent(MainActivity.this,SendMailActivity.class);
                    startActivity(i);
                }catch (Exception c){

                }finally {

                }

            }
        };
    }

    public void startService(){

        Intent serviceIntent = new Intent(this,MailNotificationService.class);
        //serviceIntent.putExtra();

        startService(serviceIntent);
    }

    public void stopService(){
        Intent serviceIntent = new Intent(this,MailNotificationService.class);
        stopService(serviceIntent);
    }
    public void IniciarImap(){
        Thread t =  new Thread(){
            public void run(){
                try{
                    String res;
                    res = IniciarImap(MainActivity.getEmail(),MainActivity.getPassword());
                    String inbox = "";
                    if(res.contains("Confirmado")){
                        inbox = listarCorreos();
                    }
                    Intent i = new Intent(MainActivity.this,MailActivity.class);
                    i.putExtra("lista",inbox);
                    startActivity(i);
                }catch (Exception c){

                }finally {

                }

            }
        };
    }
    @Override
    public void onClick(View v) {
        Toast.makeText(getApplicationContext(),"iniciando conexion",Toast.LENGTH_SHORT).show();
        switch (v.getId()){
            case R.id.btIngresar:
                String mail = tvMail.getText().toString();
                String password = tvPassword.getText().toString();

                String respuesta = IniciarCorreo(mail,password);
                Toast.makeText(getApplicationContext(), respuesta,Toast.LENGTH_LONG).show();
                if(!respuesta.contains("Confirmado"))
                    return;
                if(cbSave.isChecked())
                    saveData();

                Intent i = new Intent(this,SendMailActivity.class);
                startActivity(i);
                break;
        }
    }
}

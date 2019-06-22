package com.example.smtpclient;

import android.Manifest;
import android.content.ContentResolver;
import android.content.ContentUris;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.provider.DocumentsContract;
import android.provider.MediaStore;
import android.provider.OpenableColumns;
import android.support.annotation.Nullable;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.webkit.MimeTypeMap;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;

public class SendMailActivity extends AppCompatActivity implements View.OnClickListener, ListView.OnItemClickListener{

    EditText etMailTo;
    EditText etMailContent;
    EditText etMailSubject;
    Button btSend;
    Button btCancel;
    ImageButton btFile;
    ListView lvAttachments;
    static {
        System.loadLibrary("native-lib");
    }
    ArrayAdapter<String> adapter;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_send_mail);
        etMailContent = findViewById(R.id.etContent);
        etMailTo = findViewById(R.id.etMailTo);
        etMailSubject = findViewById(R.id.etMailSubject);
        btSend = findViewById(R.id.btSend);
        btSend.setOnClickListener(this);
        btCancel = findViewById(R.id.btCancel);
        btCancel.setOnClickListener(this);
        btFile = findViewById(R.id.btAttachFile);
        btFile.setOnClickListener(this);
        lvAttachments = findViewById(R.id.lvAttachments);

        adapter = new ArrayAdapter<>(this,android.R.layout.simple_expandable_list_item_1,attachments);
        lvAttachments.setAdapter(adapter);
        lvAttachments.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, final int position, long id) {
                AlertDialog.Builder adb = new AlertDialog.Builder(SendMailActivity.this);
                adb.setMessage("Desea eliminar: " + attachments.get(position) )
                        .setCancelable(false)
                        .setPositiveButton("Si", new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialog, int which) {
                                        String name = attachments.get(position);
                                        RemoveFile(name);
                                        attachments.remove(position);
                                        adapter.notifyDataSetChanged();
                                        mailSize -= sizes.get(name);
                                    }
                                })
                        .setNegativeButton("No", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {

                            }
                        });
                    adb.create().show();
            }
        });
    }
    public native String SendMail(String to,String subject,String message);
    public native void logout();
    public native void RemoveFile(String file);
    public native void AddRCPT(String mail);
    public native void AddFile(String filename,String path,String mimetype);
    public final long MAXSIZE = 26214000;
    ArrayList<String> attachments = new ArrayList<String>();
    HashMap<String, Integer> sizes = new HashMap<>();
    private long mailSize = 0;
    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        if(requestCode==10){
            if(resultCode==RESULT_OK){
                if (ContextCompat.checkSelfPermission(this,
                        Manifest.permission.READ_EXTERNAL_STORAGE)
                        != PackageManager.PERMISSION_GRANTED) {
                    if (ActivityCompat.shouldShowRequestPermissionRationale(this,
                            Manifest.permission.READ_EXTERNAL_STORAGE)) {
                    } else {
                        ActivityCompat.requestPermissions(this,
                                new String[]{Manifest.permission.READ_EXTERNAL_STORAGE},0);
                    }
                }




                Uri returnUri = data.getData();
                String mimeType = getContentResolver().getType(returnUri);
                String real = RealPathUtil.getRealPath(this,returnUri);
                Cursor returnCursor =
                        getContentResolver().query(returnUri, null, null, null, null);

                int nameIndex = returnCursor.getColumnIndex(OpenableColumns.DISPLAY_NAME);
                int sizeIndex = returnCursor.getColumnIndex(OpenableColumns.SIZE);
                returnCursor.moveToFirst();
                String name = returnCursor.getString(nameIndex);
                long size = returnCursor.getLong(sizeIndex);
                if(mailSize + size < MAXSIZE){
                    mailSize += size;

                    AddFile(name,real, mimeType);
                    attachments.add(name);
                    sizes.put(name,(int)size);
                    adapter.notifyDataSetChanged();

                }else{
                    Toast.makeText(this,"Sobrepaso el limite de archivos",Toast.LENGTH_SHORT).show();
                }
            }
        }
    }

    private String getRealPathFromURI(Uri contentURI) {
        String filePath;
        Cursor cursor = getContentResolver().query(contentURI, null, null, null, null);
        if (cursor == null) {
            filePath = contentURI.getPath();
        } else {
            cursor.moveToFirst();
            int idx = cursor.getColumnIndex(MediaStore.Images.ImageColumns.DATA);
            filePath = cursor.getString(idx);
            cursor.close();
        }
        return filePath;
    }

    @Override
    public void onClick(View v) {

        switch (v.getId()){
            case R.id.btSend:
                String to = etMailTo.getText().toString();
                String message = etMailContent.getText().toString();
                String subject = etMailSubject.getText().toString();
                String[] recipients = to.split(" ");
                for(String rcpt : recipients){
                    AddRCPT(rcpt);
                }

                String resultado = SendMail(to,subject,message);
                Toast.makeText(getApplicationContext(),resultado,Toast.LENGTH_SHORT).show();
                break;
            case R.id.btCancel:
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
            case R.id.btAttachFile:
                Intent picker = new Intent(Intent.ACTION_GET_CONTENT);
                picker.setType("*/*");
                startActivityForResult(picker,10);

                break;
            case R.id.lvAttachments:

        }

    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

    }
}

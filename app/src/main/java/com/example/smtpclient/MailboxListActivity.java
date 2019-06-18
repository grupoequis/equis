package com.example.smtpclient;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;

public class MailboxListActivity extends AppCompatActivity {


    ListView lvMailboxList;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_mailbox_list);
        lvMailboxList = findViewById(R.id.lvMailBox);
        Intent i = getIntent();
        Bundle b = i.getExtras();
        String lista[] = (String[]) b.get("lista");

        ArrayAdapter<String> adapter = new ArrayAdapter<>(this,android.R.layout.simple_expandable_list_item_1,lista);

        lvMailboxList.setAdapter(adapter);
    }

}

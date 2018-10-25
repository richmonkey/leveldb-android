package com.example.leveldb;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import com.google.code.p.leveldb.LevelDB;
import com.google.code.p.leveldb.LevelDBIterator;

import java.io.File;

public class DBActivity extends Activity {
    /**
     * Called when the activity is first created.
     */
    private final String TAG="leveldb";
    private LevelDB db;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        String dir = getFilesDir().getAbsoluteFile() + File.separator + "db2";
        db = LevelDB.getDefaultDB();
        db.open(dir);
        Log.i(TAG, "path:" + dir);
        try {
            db = LevelDB.getDefaultDB();
            db.set("key2", "value2");
            String v = db.get("key2");
            if (v != null) {
                Log.i(TAG, "value:" + v);
            } else {
                Log.i(TAG, "value is null");
            }
        } catch (Exception e){

        }
        try {
            LevelDBIterator iter = db.newIterator();
            iter.seekToFirst();
            while (iter.isValid()) {
                Log.i(TAG, "key:" + iter.getKey() + " value:" + iter.getValue());
                iter.next();
            }
            iter.close();
        } catch (Exception e) {

        }

    }
}

package com.hello.goodreminder;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Rect;
import android.os.Bundle;
import android.view.Window;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

import org.libsdl.app.SDLActivity;


public class MainActivity extends SDLActivity {

    static {
        System.loadLibrary("goodreminder");
    }

    public void onCreate(Bundle savedConfigs){
        getWindow().requestFeature(Window.FEATURE_NO_TITLE);
        super.onCreate(savedConfigs);
    }
    @Override
    protected String[] getLibraries() {

        try {
            String[] list = getAssets().list("images");

            for (String s : list) {
                LoadImageContents("images/" + s);
            }
        }
        catch(IOException e){
            System.out.println(e.getMessage());
        }

        try {
            String[] list = getAssets().list("fonts");

            for(String s : list){
                LoadFontContent("fonts/" + s);
            }
        }
        catch(IOException e){
            System.out.println(e.getMessage());
        }

        SetSavingDirectory(getFilesDir().getPath().getBytes());

        return new String[]{
                "SDL2",
                "goodreminder"
        };

    }

    public float getKeyboardScreenHeight() {
        Rect rect = new Rect();
        getWindow().getDecorView().getWindowVisibleDisplayFrame(rect);



        float height = getWindow().getDecorView().getHeight() - (rect.bottom);
        return height;
    }

    public void LoadImageContents(String imageLoc){

        InputStream stream;
        try {
            stream = getAssets().open(imageLoc);
        }
        catch(IOException e){
            return;
        }


        Bitmap bitmap = BitmapFactory.decodeStream(stream);

        int width = bitmap.getWidth();
        int height = bitmap.getHeight();

        if(width == 0){
            return;
        }


        ByteBuffer buffer = ByteBuffer.allocate(bitmap.getByteCount());

        bitmap.copyPixelsToBuffer(buffer);

        buffer.rewind();

        LoadImage(imageLoc.getBytes(),buffer.array(),width,height);

    }

    public void LoadFontContent(String fontLoc){
        InputStream stream;
        byte[] bytes;
        try {
            stream = getAssets().open(fontLoc);
            bytes = new byte[stream.available()];

            stream.read(bytes);
            stream.close();
        }
        catch(IOException e){
            return;
        }



        LoadFont(fontLoc.getBytes(),bytes);
    }



    public native void SetSavingDirectory(byte[] path);
    public native void LoadImage(byte[] name,byte[] bytes,int width,int height);
    public native void LoadFont(byte[] name,byte[] bytes);

}



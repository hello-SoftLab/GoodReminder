package knz.goodreminder;

import android.app.NativeActivity;
import android.content.res.AssetFileDescriptor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Rect;
import android.os.Bundle;

import android.content.Context;
import android.text.Html;
import android.view.Gravity;
import android.view.inputmethod.InputMethodManager;
import android.view.KeyEvent;
import android.widget.EditText;
import android.widget.LinearLayout;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.util.concurrent.LinkedBlockingQueue;
import org.libsdl.app.SDLActivity;
import org.libsdl.app.SDL;


public class MainActivity extends SDLActivity {

    static {
        System.loadLibrary("goodreminder");
    }

    public void onCreate(Bundle savedConfigs){
        super.onCreate(savedConfigs);
    }
    @Override
    protected String[] getLibraries() {
        LoadImageContents("logo-ALLLETTERCASEOFF.png");
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

        byte[] data = new byte[width*height];

        try {
            int read = stream.read(data);
            if(read == 0) {
                return;
            }
        }
        catch(IOException e){
            return;
        }

        LoadImage(imageLoc.getBytes(),data,width,height);

    }



    public native void LoadImage(byte[] name,byte[] bytes,int width,int height);

}



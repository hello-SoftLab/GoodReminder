package knz.goodreminder;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Rect;
import android.os.Bundle;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

import org.libsdl.app.SDLActivity;


public class MainActivity extends SDLActivity {

    static {
        System.loadLibrary("goodreminder");
    }

    public void onCreate(Bundle savedConfigs){
        super.onCreate(savedConfigs);
    }
    @Override
    protected String[] getLibraries() {

        try {
            String[] list = getAssets().list("logo");

            for (String s : list) {
                LoadImageContents("logo/" + s);
            }
        }
        catch(IOException e){
            System.out.println(e.getMessage());
        }
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



    public native void LoadImage(byte[] name,byte[] bytes,int width,int height);

}



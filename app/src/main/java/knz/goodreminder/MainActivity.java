package knz.goodreminder;

import android.app.NativeActivity;
import android.graphics.Rect;
import android.os.Bundle;

import android.content.Context;
import android.text.Html;
import android.view.Gravity;
import android.view.inputmethod.InputMethodManager;
import android.view.KeyEvent;
import android.widget.EditText;
import android.widget.LinearLayout;

import java.util.concurrent.LinkedBlockingQueue;
import org.libsdl.app.SDLActivity;
import org.libsdl.app.SDL;


public class MainActivity extends SDLActivity {

    public void onCreate(Bundle savedConfigs){
        super.onCreate(savedConfigs);
    }
    @Override
    protected String[] getLibraries() {
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
}

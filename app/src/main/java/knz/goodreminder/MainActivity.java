package knz.goodreminder;

import android.app.NativeActivity;
import android.os.Bundle;

import android.content.Context;
import android.view.inputmethod.InputMethodManager;
import android.view.KeyEvent;
import java.util.concurrent.LinkedBlockingQueue;
import org.libsdl.app.SDLActivity;


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
}

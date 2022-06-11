package com.hello.goodreminder;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Rect;
import android.os.Bundle;
import android.os.Environment;
import android.view.Window;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

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
        SetSavingDirectory(getFilesDir().getPath().getBytes());

        try {
            copyDirorfileFromAssetManager("", addTrailingSlash(getFilesDir().getPath()) + addTrailingSlash("assets"));
        }catch(Exception e){
            System.out.println(e.getMessage());
        }
        return new String[]{
                "SDL2",
                "goodreminder"
        };

    }



    public native void SetSavingDirectory(byte[] path);
    public native void LoadImage(byte[] name,byte[] bytes,int width,int height);
    public native void LoadFont(byte[] name,byte[] bytes);


    public String copyDirorfileFromAssetManager(String arg_assetDir, String arg_destinationDir) throws IOException
    {

        String dest_dir_path = addLeadingSlash(arg_destinationDir);
        File dest_dir = new File(dest_dir_path);


        createDir(dest_dir);

        AssetManager asset_manager = getApplicationContext().getAssets();
        String[] files = asset_manager.list(arg_assetDir);

        for (String file : files) {

            String abs_asset_file_path = addTrailingSlash(arg_assetDir) + file;
            String[] sub_files = asset_manager.list(abs_asset_file_path);

            if (sub_files.length == 0) {
                // It is a file
                String dest_file_path = addTrailingSlash(dest_dir_path) + file;
                if(!new File(dest_file_path).exists()) {
                    copyAssetFile(abs_asset_file_path, dest_file_path);
                }
            } else {
                // It is a sub directory
                copyDirorfileFromAssetManager(abs_asset_file_path, addTrailingSlash(arg_destinationDir) + file);
            }
        }

        return dest_dir_path;
    }


    public void copyAssetFile(String assetFilePath, String destinationFilePath) throws IOException
    {
        InputStream in = getApplicationContext().getAssets().open(assetFilePath);
        OutputStream out = new FileOutputStream(destinationFilePath);

        byte[] buf = new byte[1024*256];
        int len;
        while ((len = in.read(buf)) > 0)
            out.write(buf, 0, len);
        in.close();
        out.close();
    }

    public String addTrailingSlash(String path)
    {
        if(path == ""){
            return "";
        }
        if (path.charAt(path.length() - 1) != '/')
        {
            path += "/";
        }
        return path;
    }

    public String addLeadingSlash(String path)
    {
        if (path.charAt(0) != '/')
        {
            path = "/" + path;
        }
        return path;
    }

    public void createDir(File dir) throws IOException
    {
        if (dir.exists())
        {
            if (!dir.isDirectory())
            {
                throw new IOException("Can't create directory, a file is in the way");
            }
        } else
        {
            dir.mkdirs();
            if (!dir.isDirectory())
            {
                throw new IOException("Unable to create directory");
            }
        }
    }
}



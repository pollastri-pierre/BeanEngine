package com.bean_engine;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.TreeMap;

import com.game.bean_project.R;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.os.Bundle;
import android.util.Log;

public class BeanConfigurationSetter extends Activity {

	public static final String PREF_NAME = "Global Preferences";
	public static final String TIMESTAMP = "ApkTimestamp";
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.loading);
	}
	
	protected void debug(String msg) {
		Log.d("BeanEngine", msg);
	}
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);
	}
	
	@Override
	protected void onResume() {
		super.onResume();
		if (needSetup()) {
			(new SetupTask()).start();
		} else
			startNativeActivity();
	}

	protected void startNativeActivity() {
		Intent intent = new Intent(this, BEngineAcitivity.class);
		startActivity(intent);
	}
	
	protected boolean needSetup() {
		SharedPreferences pref = getSharedPreferences(PREF_NAME, 0);
		return !(pref.contains(TIMESTAMP) && pref.getLong(TIMESTAMP, 0) == getApkTimestamp());
	}

	
	protected long getApkTimestamp() {
		PackageManager m = getPackageManager();
		String s = getPackageName();
		PackageInfo p;
		try {
			p = m.getPackageInfo(s, 0);
			s = p.applicationInfo.sourceDir;
			File apk = new File(s);
			return apk.lastModified();
		} catch (NameNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return -1;
	}

	private class SetupTask extends Thread {
		
		@Override
		public void run() {
			super.run();
			copyAssets("scripts");
			SharedPreferences.Editor editor = getSharedPreferences(PREF_NAME, 0).edit();
			editor.putLong(TIMESTAMP, getApkTimestamp());
			editor.apply();
			BeanConfigurationSetter.this.runOnUiThread(new Runnable() {
				
				@Override
				public void run() {
					startNativeActivity();
				}
			});
		}
		
		private String getExternalDir() {
			File f = getApplicationContext().getFilesDir(); 
			return f.toString();
		}
		
		private String getScriptsDir() {
			return getExternalDir() + "/";
		}
		
		private void copyAssets(String path) {
		    AssetManager assetManager = BeanConfigurationSetter.this.getAssets();
		    String assets[] = null;
		    try {
		        assets = assetManager.list(path);
		        if (assets.length == 0)
		        	copyFile(path);
		        else {
		        	if (!new File(getScriptsDir() + path).mkdir())
		        		debug("Failed create dir " + getScriptsDir() + path);
		        }
		        for (String asset : assets) {
		        	copyAssets(path + "/" + asset);
		        }
		    } catch (IOException ex) {
		        Log.e("tag", "I/O Exception", ex);
		        debug(ex.getMessage());
		    }
		}
		
		private void copyFile(String filename) {
		    AssetManager assetManager = BeanConfigurationSetter.this.getAssets();

		    InputStream in = null;
		    OutputStream out = null;
		    try {
		        in = assetManager.open(filename);
		        String newFileName = getScriptsDir() + filename;
		        out = new FileOutputStream(newFileName);
		        debug("Path = " + newFileName);
		        byte[] buffer = new byte[1024];
		        int read;
		        while ((read = in.read(buffer)) != -1) {
		            out.write(buffer, 0, read);
		        }
		        in.close();
		        in = null;
		        out.flush();
		        out.close();
		        out = null;
		    } catch (Exception e) {
		        Log.e("tag", e.getMessage());
		        debug(e.getMessage());
		    }

		}
		
	}
	
}

package com.bean_engine;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import com.bean_engine.sound_engine.BSoundEngine;

import android.app.NativeActivity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetManager;
import android.media.AudioManager;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

public class BEngineAcitivity extends NativeActivity {
	
	BSoundEngine engine = null;
	
	@Override
	protected void onStart() {
		// TODO Auto-generated method stub
		super.onStart();
		setVolumeControlStream(AudioManager.STREAM_MUSIC);
		//setMulticastAccess();
	}
	
	protected void debug(String msg) {
		Log.d("BeanEngine", msg);
	}
	
	public String getStorageDirectory()
	{
		File f = getApplicationContext().getFilesDir(); 
		return f.toString();
	}
	
	public void setMulticastAccess()
	{
		WifiManager mgr = (WifiManager) getSystemService(Context.WIFI_SERVICE);
		WifiManager.MulticastLock lock = mgr.createMulticastLock("BeanEngine_MulticastLock");
		lock.acquire();
		lock.release();
	}
	
	
	public BSoundEngine getSoundEngine() {
		return engine = (engine != null) ? engine :  new BSoundEngine(this);
	}
	
	public void stopSoundEngine() {
		engine = null;
	}
}

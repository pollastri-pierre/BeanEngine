package com.bean_engine.sound_engine;

import java.io.FileDescriptor;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.util.Log;

public class BSoundEngine {
	
	private static class SoundInfo {
		public String name;
		public String path;
		public short volume;
		public boolean loop;
		public int id;
		
		public SoundInfo(String name, String path, short volume, boolean loop, int id) {
			this.name = name;
			this.path = path;
			this.volume = volume;
			this.loop = loop;
			this.id = id;
		}
		
	}
	
	private SoundPool _pool;
	private Map<Integer, SoundInfo> _bgms = new HashMap<Integer, SoundInfo>();
	
	private Context _context;
	
	public BSoundEngine(Context context) {
		_pool = new SoundPool(16, AudioManager.STREAM_MUSIC, 0);
		_context = context;
		
	}
	/*
	public void playBGM(String name, String path, int level, short volume, boolean expansion, boolean loop) {
		try {
			if (_bgms.get(level) != null && _bgms.get(level).path == path) {
				SoundInfo bgm = _bgms.get(level);
				bgm.volume = volume;
				bgm.loop = loop;
			} else {
				SoundInfo bgm;
				if (_bgms.get(level) != null) {
					bgm = new BGMInfo(name, path, volume, loop, _bgms.get(level).level);
					bgm.player.reset();
				} else
					bgm = new BGMInfo(name, path, volume, loop, new MediaPlayer());
				AssetFileDescriptor fd = openSound(path, expansion);
				try {
					bgm.player.reset();
					bgm.player.setDataSource(fd.getFileDescriptor(), fd.getStartOffset(), fd.getLength());
					bgm.player.prepare();
					_bgms.put(level, bgm);
				} catch (IllegalArgumentException e) {
					Log.e("BeanEngine", Log.getStackTraceString(e));
				} catch (IllegalStateException e) {
					Log.e("BeanEngine", Log.getStackTraceString(e));
				} catch (IOException e) {
					Log.e("BeanEngine", Log.getStackTraceString(e));
				}
				
			}
			playBGM(level);
		} catch (Exception e) {
			Log.e("BeanEngine", Log.getStackTraceString(e));
		}
		
	}
	
	
	public void playBGM(int level) {
		BGMInfo bgm = _bgms.get(level);
		if (bgm != null) {
			float volume = bgm.volume / 100.f;
			bgm.player.setLooping(bgm.loop);
			bgm.player.setVolume(volume, volume);
			bgm.player.start();
		}
	}
	
	public void playBGM(String level) {
		Log.e("BeanEngine", "Un test de son " + level);
	}
	
	private AssetFileDescriptor openSound(String path, boolean expansion) {
		AssetFileDescriptor out = null;
		if (expansion) {
			
		} else {
			try {
				Log.e("BeanEngine", "PATH  = " + path);
				out = _context.getAssets().openFd(path);
				
			} catch (Exception e) {
				Log.e("BeanEngine", Log.getStackTraceString(e));
			}
		}
		return out;
	}
	*/
}

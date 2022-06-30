package com.cfwang.wcdemo;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.media.AudioAttributes;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioRecord;
import android.media.AudioTrack;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.MediaStore;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.SeekBar;

import androidx.annotation.Nullable;
import androidx.core.app.ActivityCompat;
import androidx.navigation.NavDirections;
import androidx.navigation.Navigation;
import com.cfwang.wcdemo.R;
import com.cfwang.wavechannel.Native;

import java.nio.charset.StandardCharsets;

import butterknife.BindView;

public class SendFragment extends BaseFragment {
    @BindView(R.id.btn_play)
    Button playButton;
    @BindView(R.id.edit_text)
    EditText editText;
    public static int mViewId = 0;
    private static int AUDIO_RATE = 16000;
    private static int AUDIO_BITS = 16;
    private PlayThread mPlayThread = new PlayThread();
    @Override
    protected int getLayoutId() {
        return R.layout.fragment_send;
    }

    @Override
    protected void initData(Bundle savedInstanceState) {
        mViewId = this.getId();
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    protected void initView() {
        getAppActivity().getSupportActionBar().setDisplayHomeAsUpEnabled(false);
        playButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String text = editText.getText().toString();
                if(text.isEmpty()){
                    return;
                }
                mPlayThread.stopPlay();
                mPlayThread.startPlay(text);
            }
        });
    }
    @Override
    protected void onViewDestroy(){

    }

    @Override
    public void onPause() {
        mPlayThread.stopPlay();
        super.onPause();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }
    private class PlayThread implements Runnable {
        private boolean mIsPlaying = false;
        private Thread mThread;
        private String mPlayText;
        @Override
        public void run() {
            int ret;
            long handle = Native.waveTransSendCreateHander(AUDIO_BITS,AUDIO_RATE);
            if(handle == 0){
                return;
            }
            Native.PcmInfo pcm = new Native.PcmInfo();
            ret = Native.waveTransSendGetPcm(handle,
                    mPlayText.getBytes(StandardCharsets.UTF_8),pcm);
            if(ret != 0){
                Native.waveTransSendDestroyHander(handle);
                return;
            }
            int bits = AUDIO_BITS == 16 ? AudioFormat.ENCODING_PCM_16BIT :
                    AudioFormat.ENCODING_PCM_8BIT;
            int playBufSize = AudioRecord.getMinBufferSize(AUDIO_RATE,
                    AudioFormat.CHANNEL_IN_MONO, bits);
            AudioTrack track = new AudioTrack(new AudioAttributes.Builder()
                    .setUsage(AudioAttributes.USAGE_MEDIA)
                    .setContentType(AudioAttributes.CONTENT_TYPE_MUSIC)
                    .build(),
                    new AudioFormat.Builder().setSampleRate(AUDIO_RATE)
                            .setEncoding(bits)
                            .setChannelMask(AudioFormat.CHANNEL_OUT_MONO)
                            .build(),
                    playBufSize,
                    AudioTrack.MODE_STREAM,
                    AudioManager.AUDIO_SESSION_ID_GENERATE);
            track.play();
            int readSize = 0;
            while(mIsPlaying && readSize < pcm.pcmBuf.length){
                int playSize = Math.min(pcm.pcmBuf.length - readSize, playBufSize);
                track.write(pcm.pcmBuf,readSize,playSize);
                readSize += playSize;
            }
            track.flush();
            int waitMs = 1000;
            int curMs = 0;
            while(mIsPlaying && curMs < waitMs){
                try{
                    Thread.sleep(1);
                }catch (InterruptedException e){}
                curMs++;
            }
            track.release();
            Native.waveTransSendDestroyHander(handle);
        }
        public int startPlay(String text){
            if(mIsPlaying){
                return -1;
            }
            mPlayText = text;
            mIsPlaying = true;
            mThread = new Thread(this);
            mThread.start();
            return 0;
        }
        private void stopPlay(){
            if(!mIsPlaying){
                return;
            }
            mIsPlaying = false;
            try {
                mThread.join();
            }catch (InterruptedException e){}
            mThread = null;
        }
    }
}
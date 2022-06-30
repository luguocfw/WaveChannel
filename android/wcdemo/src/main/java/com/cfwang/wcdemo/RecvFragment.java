package com.cfwang.wcdemo;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Rect;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.core.app.ActivityCompat;
import androidx.navigation.NavDirections;
import androidx.navigation.Navigation;

import com.cfwang.wavechannel.Native;
import com.cfwang.wcdemo.R;

import org.w3c.dom.Text;

import java.io.UnsupportedEncodingException;

import butterknife.BindView;

public class RecvFragment extends BaseFragment {
    @BindView(R.id.btn_start)
    Button startButton;
    @BindView(R.id.btn_stop)
    Button stopButton;
    @BindView(R.id.btn_clean)
    Button cleanButton;
    @BindView(R.id.tex_data)
    TextView dataView;
    @BindView(R.id.tex_status)
    TextView statusView;
    public static int mViewId = 0;
    public boolean mIsRecving = false;
    private static int REQUEST_PERMISSION_CODE = 3;
    private RecvThread mRecvThread = new RecvThread();
    private byte[] mShowText = new byte[1024*1024];
    private int mShowTextSize = 0;

    @Override
    protected int getLayoutId() {
        return R.layout.fragment_recv;
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
        if (ActivityCompat.checkSelfPermission(this.getActivity(), Manifest.permission.RECORD_AUDIO)
                != PackageManager.PERMISSION_GRANTED) {
            String[] arr = new String[1];
            arr[0] = Manifest.permission.RECORD_AUDIO;
            ActivityCompat.requestPermissions(this.getActivity(),
                    arr, REQUEST_PERMISSION_CODE);
        }
        if (ActivityCompat.checkSelfPermission(this.getActivity(), Manifest.permission.RECORD_AUDIO)
                != PackageManager.PERMISSION_GRANTED) {
            return;
        }
        startButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startRecv();
            }
        });
        stopButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                stopRecv();
            }
        });
        cleanButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                cleanData();
            }
        });
    }

    @Override
    protected void onViewDestroy() {

    }

    @Override
    public void onPause() {
        stopRecv();
        super.onPause();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }

    private void startRecv() {
        if (mIsRecving) {
            return;
        }
        int ret;
        ret = mRecvThread.startRecv();
        if(ret != 0){
            return;
        }
        mIsRecving = true;
        updateRecvStatus(mIsRecving);
    }
    private void stopRecv(){
        if(!mIsRecving){
            return;
        }
        mRecvThread.stopRecv();
        mIsRecving = false;
        updateRecvStatus(mIsRecving);
    }
    private void cleanData(){
        dataView.setText("");
        mShowTextSize = 0;
    }
    private void showRecvData(byte[] data,int len) {
        if(mShowTextSize + len > 1024*1024){
            mShowTextSize = 0;
        }
        System.arraycopy(data,0,mShowText,mShowTextSize,len);
        mShowTextSize += len;
        getActivity().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                try {
                    String str = new String(mShowText, 0, mShowTextSize, "UTF-8");
                    dataView.setText(str);
                }catch(UnsupportedEncodingException e){}
            }
        });
    }
    private void updateRecvStatus(boolean isRecving){
        if(isRecving){
            statusView.setText("正在接收...");
            cleanData();
        }else{
            statusView.setText("未开始");
        }
    }
    private class RecvThread implements ThreadBase.MyRunnable {
        private ThreadBase mRecThread = new ThreadBase();
        private ThreadBase mDecThread = new ThreadBase();
        private boolean mIsRunning = false;
        private int mRecBufSize = 0;
        private AudioRecord mAudioRec;
        private int THREAD_ID_REC = 1;
        private int THREAD_ID_DECODE = 2;
        @Override
        public void run(int id) {
            if(id == THREAD_ID_REC){
                runForRecord();
            }else if(id == THREAD_ID_DECODE){
                runForDecode();
            }
        }
        private void runForRecord(){
            byte[] buffer = new byte[mRecBufSize];
            while(mIsRunning){
                int read = mAudioRec.read(buffer,0,mRecBufSize);
                if(read !=  AudioRecord.ERROR_INVALID_OPERATION){
                    //Log.d("wcdemo","push pcm size:" + read);
                    Native.waveTransRecvSetPcm(buffer);
                }
            }
        }
        private void runForDecode(){
            byte[] data = new byte[1024];
            while(mIsRunning){
                int read = Native.waveTransRecvGetContext(data);
                if(read > 0){
                    showRecvData(data,read);
                }else{
                    try {
                        Thread.sleep(1);
                    }catch (InterruptedException e){}
                }
            }
        }
        public int startRecv(){
            if(mIsRunning){
                return 0;
            }
            int ret;
            ret = Native.waveTransRecvInit();
            if(ret != 0){
                return ret;
            }
            mRecBufSize = AudioRecord.getMinBufferSize(16000,
                    AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT);
            mAudioRec = new AudioRecord(MediaRecorder.AudioSource.MIC,16000,
                    AudioFormat.CHANNEL_IN_MONO,AudioFormat.ENCODING_PCM_16BIT,mRecBufSize);
            mAudioRec.startRecording();
            mIsRunning = true;
            mRecThread.start(THREAD_ID_REC,this);
            mDecThread.start(THREAD_ID_DECODE,this);
            return 0;
        }
        public void stopRecv() {
            if(!mIsRunning){
                return;
            }
            mAudioRec.stop();
            mIsRunning = false;
            mRecThread.stop();
            mDecThread.stop();
            mAudioRec.release();
            mAudioRec = null;
        }
    }
}
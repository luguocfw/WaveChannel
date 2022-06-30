package com.cfwang.wcdemo;

import androidx.annotation.NonNull;

public class ThreadBase implements Runnable {
    public interface MyRunnable {
        void run(int id);
    }
    Thread mThread;
    MyRunnable mRunnable;
    int mId;
    @Override
    public void run() {
        mRunnable.run(mId);
    }
    public void start(int id,@NonNull MyRunnable runnable){
        mThread = new Thread(this);
        mId = id;
        mRunnable = runnable;
        mThread.start();
    }
    public void stop(){
        try {
            mThread.join();
        }catch (InterruptedException e){}
        mRunnable = null;
        mId = 0;
        mThread = null;
    }
}

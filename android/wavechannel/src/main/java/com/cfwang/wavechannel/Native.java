package com.cfwang.wavechannel;

import androidx.annotation.NonNull;

public class Native {
    public static class PcmInfo {
        public byte[] pcmBuf;
        public int rate;
        public int bits;
    }
    public static class WavInfo {
        public byte[] wavBuf;
    }
    public static native long waveTransSendCreateHander(int bits,int rate);
    public static native void waveTransSendDestroyHander(long handle);
    public static native int waveTransSendGetPcm(long handle,byte[] context,PcmInfo pcmOut);
    public static native int waveTransSendGetWav(long handle,byte[] context,WavInfo wavOut);

    public static native int waveTransRecvInit();
    public static native void waveTransRecvExit();
    public static native void waveTransRecvSetPcm(byte[] pcm);
    public static native int waveTransRecvGetContext(byte[] contextBuf);

    static {
        System.loadLibrary("wavetrans");
    }
}
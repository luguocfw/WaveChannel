package com.cfwang.wcdemo;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.MediaStore;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;

import androidx.annotation.Nullable;
import androidx.core.app.ActivityCompat;
import androidx.navigation.NavDirections;
import androidx.navigation.Navigation;
import com.cfwang.wcdemo.R;

import butterknife.BindView;

public class HomeFragment extends BaseFragment {
    @BindView(R.id.btn_sender)
    Button senderButton;
    @BindView(R.id.btn_receiver)
    Button receiverButton;

    public static int mViewId = 0;
    @Override
    protected int getLayoutId() {
        return R.layout.fragment_home;
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
        senderButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                NavDirections action = HomeFragmentDirections.actionHomeFragmentToSendFragment();
                Navigation.findNavController(view).navigate(action);
            }
        });
        receiverButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                NavDirections action = HomeFragmentDirections.actionHomeFragmentToRecvFragment();
                Navigation.findNavController(v).navigate(action);
            }
        });
    }
    @Override
    protected void onViewDestroy(){

    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }
}
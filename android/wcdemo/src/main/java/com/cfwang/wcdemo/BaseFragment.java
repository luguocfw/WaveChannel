package com.cfwang.wcdemo;


import android.app.Activity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.Fragment;

import butterknife.ButterKnife;
import butterknife.Unbinder;


public abstract class BaseFragment extends Fragment {
    private Unbinder unBinder;
    protected Activity activity;

    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        this.activity = activity;
    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View rootView = inflater.inflate(this.getLayoutId(), container, false);
        if (rootView != null) {
            unBinder = ButterKnife.bind(this, rootView);
        }
        return rootView;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        initView();
        initData(savedInstanceState);
    }

    protected AppCompatActivity getAppActivity() {
        return (AppCompatActivity) getActivity();
    }

    protected abstract int getLayoutId();

    protected abstract void initData(Bundle savedInstanceState);

    protected abstract void initView();

    protected abstract void onViewDestroy();


    @Override
    public void onDestroy() {
        super.onDestroy();
        unBinder.unbind();
    }

    @Override
    public void onDestroyView() {
        onViewDestroy();
        super.onDestroyView();
    }

}

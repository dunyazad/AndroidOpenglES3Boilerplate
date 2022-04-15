package com.artificialnature.androidopengles3boilerplate;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.artificialnature.androidopengles3boilerplate.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    OpenGLES3View mView;

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mView = new OpenGLES3View(getApplication());
        setContentView(mView);
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }
}

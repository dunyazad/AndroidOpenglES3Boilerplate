package com.artificialnature.androidopengles3boilerplate;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class OpenGLES3ViewRenderer implements GLSurfaceView.Renderer {
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        OpenGLES3JNILibrary.initialize();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        OpenGLES3JNILibrary.resize(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        OpenGLES3JNILibrary.frame();
    }
}

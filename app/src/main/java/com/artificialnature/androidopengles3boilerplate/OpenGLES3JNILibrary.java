package com.artificialnature.androidopengles3boilerplate;

public class OpenGLES3JNILibrary {
    static {
        System.loadLibrary("opengles3native");
    }

    public static native void initialize();
    public static native void resize(int width, int height);
    public static native void frame();
}

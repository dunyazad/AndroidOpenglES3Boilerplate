package com.artificialnature.androidopengles3boilerplate;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

public class OpenGLES3View extends GLSurfaceView {
    public OpenGLES3View(Context context) {
        super(context);

        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        setEGLContextClientVersion(3);
        setRenderer(new OpenGLES3ViewRenderer());
    }

    public OpenGLES3View(Context context, AttributeSet attrs) {
        super(context, attrs);
    }
}

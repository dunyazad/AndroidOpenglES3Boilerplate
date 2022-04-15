#include <jni.h>
#include <string>
#include <GLES3/gl32.h>

extern "C"
JNIEXPORT void JNICALL
Java_com_artificialnature_androidopengles3boilerplate_OpenGLES3JNILibrary_initialize(
        JNIEnv *env, jclass clazz) {
}
extern "C"
JNIEXPORT void JNICALL
Java_com_artificialnature_androidopengles3boilerplate_OpenGLES3JNILibrary_resize(JNIEnv *env,
                                                                                 jclass clazz,
                                                                                 jint width,
                                                                                 jint height) {
    glViewport(0, 0, width, height);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_artificialnature_androidopengles3boilerplate_OpenGLES3JNILibrary_frame(JNIEnv *env,
                                                                                jclass clazz) {
    glClearColor(0.5f, 0.7f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

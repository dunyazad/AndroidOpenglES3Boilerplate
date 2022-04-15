#include <jni.h>
#include <string>
#include <GLES3/gl32.h>
#include <android/log.h>

#define DEBUG 1

#define LOG_TAG "OpenGLES3JNILibrary"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

using namespace std;

void InitializeScene();
void InitializeShader();
void InitalizeGeometry();
void InitalizeTexture();
void TerminateScene();
void Frame();
void RenderScene();

extern "C"
JNIEXPORT void JNICALL
Java_com_artificialnature_androidopengles3boilerplate_OpenGLES3JNILibrary_initialize(
        JNIEnv *env,
        jclass clazz) {
    InitializeScene();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_artificialnature_androidopengles3boilerplate_OpenGLES3JNILibrary_terminate(
        JNIEnv *env,
        jclass clazz) {
    TerminateScene();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_artificialnature_androidopengles3boilerplate_OpenGLES3JNILibrary_resize(
        JNIEnv *env,
        jclass clazz,
        jint width,
        jint height) {
    glViewport(0, 0, width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_artificialnature_androidopengles3boilerplate_OpenGLES3JNILibrary_frame(
        JNIEnv *env,
        jclass clazz) {
    Frame();
}




unsigned long long int g_LastFrameNs = 0;

unsigned int VBO_Vertex, VBO_TexCoord, VAO;

float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,

        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
};

float texcoords[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
};

unsigned int shaderProgram;

const char* vertexShaderSource = "#version 320 es\n"
                                 "in vec3 aPos;\n"
                                 "in vec2 aTexCoords;\n"
                                 "out vec2 vTexCoords;\n"
                                 "\n"
                                 "void main()\n"
                                 "{\n"
                                 "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}";

const char* fragmentShaderSource = "#version 320 es\n"
                                   "precision highp float;"
                                   "in vec2 vTexCoords;"
                                   "out vec4 FragColor;\n"
                                   "uniform sampler2D texture0;"
                                   "\n"
                                   "void main()\n"
                                   "{\n"
//                                   "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//                                   "    FragColor = vec4(vTexCoords.x, vTexCoords.y, 0.2f, 1.0f);\n"
                                   "FragColor = texture(texture0, vTexCoords);"
                                   "}";

unsigned char textureData[1024 * 1024 * 4];

GLuint textureID = -1;

void InitializeScene() {
    InitializeShader();
    InitalizeGeometry();
    InitalizeTexture();
}

void InitializeShader() {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        ALOGE("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        ALOGE("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        ALOGE("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void InitalizeGeometry() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO_Vertex);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Vertex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);




    glGenBuffers(1, &VBO_TexCoord);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_TexCoord);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);


    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}

void InitalizeTexture() {
    for (int y = 0; y < 1024; ++y) {
        for (int x = 0; x < 1024; ++x) {
            textureData[y * 1024 * 4 + x * 4 + 0] = (unsigned char)(255 * (float)x / 1024.f);
            textureData[y * 1024 * 4 + x * 4 + 1] = (unsigned char)(255 * (float)y / 1024.f);
            textureData[y * 1024 * 4 + x * 4 + 2] = (unsigned char)(255 * (float)(x + y) / (1024.f * 2.f));
            textureData[y * 1024 * 4 + x * 4 + 3] = 255;
        }
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
//    glGenerateMipmap(GL_TEXTURE_2D);

//    glBindTexture(GL_TEXTURE_2D, 0);
}

void TerminateScene() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO_Vertex);
    glDeleteBuffers(1, &VBO_TexCoord);
    glDeleteProgram(shaderProgram);
}

void Frame() {
    timespec now = {0};
    clock_gettime(CLOCK_MONOTONIC, &now);
    auto nowNs = now.tv_sec*1000000000ull + now.tv_nsec;

    if (g_LastFrameNs > 0) {
        float dt = float(nowNs - g_LastFrameNs) * 0.000000001f;

        RenderScene();
    }

    g_LastFrameNs = nowNs;
}

void RenderScene() {
    glClearColor(0.5f, 0.7f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

//
// Created by fighter on 6/20/18.
//


#include "Test19.h"


JNIEXPORT jstring JNICALL Java_Test19_concat
        (JNIEnv *env, jclass cls, jstring s1, jstring s2){

    jstring  str = env->NewStringUTF("hello jni");

    return str;
}
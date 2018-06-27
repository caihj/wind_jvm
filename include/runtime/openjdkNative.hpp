//
// Created by fighter on 6/20/18.
//

#ifndef WIND_JVM_OPENJDKNATIVE_HPP
#define WIND_JVM_OPENJDKNATIVE_HPP


#include<jni.h>
#include <string>
#include <list>
#include <map>

class Oop;

/*
using std::list;
using std::wstring;
using std::map

struct ClassNativeMethod{

private:
    void *dlhandle; // return by void *dlopen(const char *filename, int flags);

    map<std::wstring,void *> methodMap;

public:
    ClassNativeMethod(const wstring & libname,list<wstring> methods);

    void * operator()(const wstring & signature);
};


void invoke_jni_func(const wstring & signature,void * func,list<Oop *> & stacks);
*/

/*
 * return true if succ
 */

extern struct JNINativeInterface_ nativeInterface;

bool loadNativeLib(std::wstring name);

#ifdef __cplusplus
extern "C" {
#endif

jstring  NewStringUTF(JNIEnv *env,const char *utf);

#ifdef __cplusplus
}
#endif

#endif //WIND_JVM_OPENJDKNATIVE_HPP

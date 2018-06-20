//
// Created by fighter on 6/20/18.
//


#include <dlfcn.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "runtime/openjdkNative.hpp"
#include "native/java_lang_String.hpp"
#include <ffi.h>
/*


ClassNativeMethod::ClassNativeMethod(const wstring &libname,list<wstring> methods) {

    std::string _libname = wstring_to_utf8(libname);
    dlhandle = dlopen(_libname.c_str(),RTLD_NOW);
    if(dlhandle == nullptr){
        std::wcout<<L"UnsatisfiedLinkError:"<<libname<<std::endl;
        assert(false);
    }
    for(wstring method:methods){
        void * func = dlsym(dlhandle,wstring_to_utf8(method).c_str());

#ifdef DEBUG
        std::wcout<<L"load method:"<<method<<std:endl;
#endif
        if(func == nullptr){
            std::wcout<<L"UnsatisfiedLinkError:"<<libname<<std::endl;
            assert(false);
        }
        methodMap[method] = func;
    }
}


boost::function<void (list<Oop *> & stacks)> static_method;

void _invoke_jni_func_helper(list<Oop *> & stacks){
    static_method(stacks);
}

void * ClassNativeMethod::operator()(const wstring &signature) {

    auto ite = methodMap.find(signature);
    if(ite==methodMap.end()){
        return nullptr;
    }else {
        auto ret = boost::bind(invoke_jni_func, signature, (*ite).second, _1);
        static_method = ret;
        return static_cast<void *>(_invoke_jni_func_helper);
    }
}

void invoke_jni_func(const wstring & signature,void * func,list<Oop *> & stacks){




    stacks.push_back();

}
*/

jstring  NewStringUTF(JNIEnv *env,const char *utf){

    wstring ws = utf8_to_wstring(std::string(utf));

    return (jstring)java_lang_string::intern(ws);
}


//
// Created by fighter on 6/20/18.
//


#include <dlfcn.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "runtime/openjdkNative.hpp"
#include "native/java_lang_String.hpp"
#include <ffi.h>
#include <system_directory.hpp>
#include <dlfcn.h>
#include <regex>

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

void initNativeInterface();
struct __init{
    __init(){
        initNativeInterface();
    }
} ;
static __init ___init;

bool loadNativeLib(std::wstring name) {

    std::string libname = wstring_to_utf8(name);

    void *handle = dlopen(libname.c_str(), RTLD_LAZY);
    if (handle == nullptr) {
        return false;
    }

    {
        LockGuard l(Method_Pool::method_pool_lock());
        for (Method *method: Method_Pool::method_pool()) {
            if (method->is_native() && method->getNative_method().empty()) {
                wstring funcName =
                        L"Java_" + std::regex_replace(method->get_klass()->get_name(), std::wregex(L"/"), L"_") + L"_" +
                        method->get_name();
#ifdef DEBUG
                std::wcerr << L"emtpy method " << method->get_klass()->get_name() << L":" << method->get_name()
                           << std::endl;
                std::wcerr << L"descriptor: " << method->get_descriptor() << std::endl;
                std::wcerr << "funcName:" << funcName << std::endl;
#endif

                void *funcPtr = dlsym(handle, wstring_to_utf8(funcName).c_str());
                if (funcPtr != nullptr) {
#ifdef DEBUG
                    std::wcerr << funcName << L" found!!!" << std::endl;
#endif
                    int argCount = method->parse_argument_list().size();

                    //for env and this
                    argCount += 2;

                    method->setNative_method([argCount, funcPtr](list<Oop *> &_stack) -> void {

                        ffi_type **ffiArgTypes = static_cast<ffi_type **>(alloca(sizeof(ffi_type *) * argCount));
                        for (int i = 0; i < argCount; i++) {
                            ffiArgTypes[i] = &ffi_type_pointer;
                        }

                        void **ffiArgs = static_cast<void **>(alloca(sizeof(void *) * argCount));

                        //env
                        ffiArgs[0] = &nativeInterface;
                        for (int i = 1; i < argCount; i++) {
                            ffiArgs[i] = _stack.front();
                            _stack.pop_front();
                        }

                        ffi_cif cif;
                        ffi_type *returnFfiType = &ffi_type_pointer;

                        ffi_status ffiPrepStatus = ffi_prep_cif(&cif, FFI_DEFAULT_ABI, (unsigned int) argCount,
                                                                returnFfiType, ffiArgTypes);

                        if (ffiPrepStatus == FFI_OK) {
                            //生成用于保存返回值的内存
                            void * *returnPtr = NULL;
                            if (returnFfiType->size) {
                                returnPtr = static_cast<void **>(alloca(returnFfiType->size));
                            }
                            //根据cif函数原型，函数指针，返回值内存指针，函数参数数据调用这个函数
                            ffi_call(&cif, reinterpret_cast<void (*)(void)>(funcPtr), returnPtr, ffiArgs);

                            //拿到返回值
                            void *returnValue = *returnPtr;
                            _stack.push_back(static_cast<Oop *>(returnValue));
                        }

                    });
                }
            }
        }
    }


};


struct JNINativeInterface_ nativeInterface;

void initNativeInterface() {
    memset(&nativeInterface,0,sizeof(struct JNINativeInterface_));

    JNIEnv *env = reinterpret_cast<JNIEnv *>(&nativeInterface);
    env->functions = &nativeInterface;
    nativeInterface.NewStringUTF = &NewStringUTF;
}


jstring NewStringUTF(JNIEnv *env, const char *utf) {

    wstring ws = utf8_to_wstring(std::string(utf));
    return static_cast<jstring>(java_lang_string::intern_to_oop(ws));
}


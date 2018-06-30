//
// Created by fighter on 6/25/18.
//


#include "native/java_lang_ClassLoader_NativeLibrary.hpp"

#include <unordered_map>
#include <native/java_lang_String.hpp>
#include <system_directory.hpp>
#include <runtime/openjdkNative.hpp>
#include "native/native.hpp"

static unordered_map<wstring, void *> methods={

        {L"load:(" STR  "Z)V",(void *)&JVM_NativeLibrary_load},
        {L"find:(" STR ")J",(void *)&JVM_NativeLibrary_find},
        {L"unload(" STR "Z)V",(void *)&JVM_NativeLibrary_unload}
};


void JVM_NativeLibrary_load(list<Oop *> & _stack){

    InstanceOop *_this = (InstanceOop *)_stack.front();	_stack.pop_front();

    //set loaded

    InstanceOop *name = (InstanceOop *)_stack.front();	_stack.pop_front();
    IntOop *isBulitIn = (IntOop *)_stack.front();	_stack.pop_front();

    wstring _name = java_lang_string::stringOop_to_wstring(name);
    if(_name == L"libzip.so"){

        //ignore zip.so
        _this->set_field_value(L"java/lang/ClassLoader$NativeLibrary:loaded:Z",new IntOop(1));
        return;
    }

    {

        int loaded = loadNativeLib(_name);
        _this->set_field_value(L"java/lang/ClassLoader$NativeLibrary:loaded:Z",new IntOop(loaded));
    }

#ifdef DEBUG
    std::wcerr << java_lang_string::stringOop_to_wstring(name) << std::endl;
    std::wcerr << isBulitIn->value << std::endl;
#endif

    std::wcerr<<"JVM_NativeLibrary_load called load:"<< java_lang_string::stringOop_to_wstring(name) <<std::endl;
}

void JVM_NativeLibrary_find(list<Oop *> & _stack){
    std::wcerr<<"JVM_NativeLibrary_find called"<<std::endl;
}

void JVM_NativeLibrary_unload(list<Oop *> & _stack){
    std::wcerr<<"JVM_NativeLibrary_unload called"<<std::endl;
}



void *java_lang_classLoader_nativeLibrary_search_method(const wstring & signature){

    auto iter = methods.find(signature);
    if (iter != methods.end()) {
        return (*iter).second;
    }
    return nullptr;
}
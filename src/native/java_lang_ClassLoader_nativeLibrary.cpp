//
// Created by fighter on 6/25/18.
//


#include "native/java_lang_ClassLoader_NativeLibrary.hpp"

#include <unordered_map>
#include "native/native.hpp"

static unordered_map<wstring, void *> methods={

        {L"load:(" STR  "Z)V",(void *)&JVM_NativeLibrary_load},
        {L"find:(" STR ")J",(void *)&JVM_NativeLibrary_find},
        {L"unload(" STR "Z)V",(void *)&JVM_NativeLibrary_unload}
};


void JVM_NativeLibrary_load(list<Oop *> & _stack){

}

void JVM_NativeLibrary_find(list<Oop *> & _stack){

}

void JVM_NativeLibrary_unload(list<Oop *> & _stack){

}



void *java_lang_classLoader_nativeLibrary_search_method(const wstring & signature){

    auto iter = methods.find(signature);
    if (iter != methods.end()) {
        return (*iter).second;
    }
    return nullptr;
}
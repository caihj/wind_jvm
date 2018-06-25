//
// Created by fighter on 6/25/18.
//

#ifndef WIND_JVM_JAVA_LANG_CLASSLOADER_NATIVELIBRARY_HPP
#define WIND_JVM_JAVA_LANG_CLASSLOADER_NATIVELIBRARY_HPP


#include "runtime/oop.hpp"
#include <list>

using std::list;

void JVM_NativeLibrary_load(list<Oop *> & _stack);
void JVM_NativeLibrary_find(list<Oop *> & _stack);
void JVM_NativeLibrary_unload(list<Oop *> & _stack);



void *java_lang_classLoader_nativeLibrary_search_method(const wstring & signature);

#endif //WIND_JVM_JAVA_LANG_CLASSLOADER_NATIVELIBRARY_HPP

//
// Created by fighter on 18-7-15.
//

#include "java_net_InetAddress.hpp"
#include <native/java_lang_Throwable.hpp>
#include "native/native.hpp"
#include "wind_jvm.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <classloader.hpp>

static unordered_map<wstring, void*> methods = {
        {L"init:()V",(void *)&init},
        {L"isIPv6Supported:()Z",(void *)&isIPv6Supported},

};


//for both
void init(list<Oop *> & _stack){

}


void isIPv6Supported(list<Oop *> & _stack){

    //not supported.
    _stack.push_back(new IntOop(0));
}


void *java_net_InetAddress_search_method(const wstring & signature){

    auto iter = methods.find(signature);
    if (iter != methods.end()) {
        return (*iter).second;
    }
    return nullptr;

}

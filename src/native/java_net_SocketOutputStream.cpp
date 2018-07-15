//
// Created by fighter on 18-7-14.
//
#include "java_net_SocketOutputStream.hpp"
#include <native/java_lang_Throwable.hpp>
#include "native/native.hpp"
#include "wind_jvm.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <classloader.hpp>



void socketWrite0(list<Oop *> & _stack){
    InstanceOop *_this = (InstanceOop *)_stack.front();	_stack.pop_front();
    InstanceOop *fileDescriptor = (InstanceOop *)_stack.front();	_stack.pop_front();

    TypeArrayOop *bytes = (TypeArrayOop *)_stack.front(); _stack.pop_front();
    int offset = ((IntOop *)_stack.front())->value;	_stack.pop_front();
    int len = ((IntOop *)_stack.front())->value;	_stack.pop_front();

    Oop *oop ;
    ((InstanceOop *)fileDescriptor)->get_field_value(FILEDESCRIPTOR L":fd:I", &oop);
    int fd = ((IntOop *)oop)->value;
    assert(bytes->get_length() > offset && bytes->get_length() >= (offset + len));

    char *buf = new char[len];

    for (int i = offset; i < offset + len; i ++) {
        buf[i] = (char)((IntOop *)(*bytes)[i])->value;
    }
    if (write(fd, buf,len) == -1) {
        assert(false);
    }
    delete[] buf;

}

static void init(list<Oop *> & _stack){

}

static unordered_map<wstring, void*> methods = {
        {L"socketWrite0:(Ljava/io/FileDescriptor;[BII)V",(void *)&socketWrite0},
        {L"init:()V",(void *)&init},
};


void *java_net_SocketOutputStream_search_method(const wstring & signature){

    auto iter = methods.find(signature);
    if (iter != methods.end()) {
        return (*iter).second;
    }
    return nullptr;

}
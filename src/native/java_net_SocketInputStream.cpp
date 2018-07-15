//
// Created by fighter on 18-7-14.
//


#include "java_net_SocketInputStream.hpp"
#include <native/java_lang_Throwable.hpp>
#include "native/native.hpp"
#include "wind_jvm.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <classloader.hpp>


static unordered_map<wstring, void*> methods = {
        {L"socketRead0:(Ljava/io/FileDescriptor;[BIII)I" ,(void *)&socketRead0},
};

void socketRead0(list<Oop *> & _stack){
    InstanceOop *_this = (InstanceOop *)_stack.front();	_stack.pop_front();
    InstanceOop *fileDescriptor = (InstanceOop *)_stack.front();	_stack.pop_front();


    TypeArrayOop *bytes = (TypeArrayOop *)_stack.front(); _stack.pop_front();
    int offset = ((IntOop *)_stack.front())->value;	_stack.pop_front();
    int len = ((IntOop *)_stack.front())->value;	_stack.pop_front();
    int timeout =  ((IntOop *)_stack.front())->value;	_stack.pop_front();

    IntOop *fd ;
    fileDescriptor->get_field_value(L"java/io/FileDescriptor:fd:I" ,reinterpret_cast<Oop **> (&fd));


    char * buf = new char[len];

    int ret = read(fd->value,buf,len);
    assert(ret!=-1);

    for(int i=offset ; i< ret ; i++){
       ((IntOop *)(*bytes)[i])->value = buf[i];
    }

    delete buf;

    _stack.push_back(new IntOop(ret));
}

void *java_net_SocketInputStream_search_method(const wstring & signature){

    auto iter = methods.find(signature);
    if (iter != methods.end()) {
        return (*iter).second;
    }
    return nullptr;

}

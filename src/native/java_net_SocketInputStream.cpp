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




static void socketRead0(list<Oop *> & _stack){
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
    if(ret < 0 ){
        auto excp_klass = ((InstanceKlass *)BootStrapClassLoader::get_bootstrap().loadClass(L"java/io/IOException"));
        // get current thread
        vm_thread *thread = (vm_thread *)_stack.back();	_stack.pop_back();
        std::wstring msg = L"read error:"+ utf8_to_wstring(strerror(errno));
        native_throw_Exception(excp_klass, thread, _stack, msg);
        return;
    }

    for(int i=offset ; i< ret ; i++){
       ((IntOop *)(*bytes)[i])->value = buf[i];
    }

    delete buf;

    _stack.push_back(new IntOop(ret));
}

static void init(list<Oop *> & _stack){

}

static unordered_map<wstring, void*> methods = {
        {L"socketRead0:(Ljava/io/FileDescriptor;[BIII)I" ,(void *)&socketRead0},
        {L"init:()V",(void *)&init},
};

void *java_net_SocketInputStream_search_method(const wstring & signature){

    auto iter = methods.find(signature);
    if (iter != methods.end()) {
        return (*iter).second;
    }
    return nullptr;

}

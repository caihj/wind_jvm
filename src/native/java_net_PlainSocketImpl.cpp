//
// Created by fighter on 18-7-8.
//

#include <native/java_lang_Throwable.hpp>
#include "java_net_PlainSocketImpl.hpp"
#include "native/native.hpp"
#include "wind_jvm.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <classloader.hpp>
#include <sys/ioctl.h>
#include <native/java_lang_String.hpp>

static unordered_map<wstring, void*> methods = {
        {L"socketCreate:(Z)V" ,(void *)&socketCreate},
        {L"socketConnect:(Ljava/net/InetAddress;II)V",(void *)&socketConnect},
        {L"socketBind:(Ljava/net/InetAddress;I)V",(void *)&socketBind},
        {L"socketListen:(I)V",(void *)&socketListen},
        {L"socketAccept:(Ljava/net/SocketImpl;)V" ,(void *)&socketAccept},
        {L"socketAvailable:()I" ,(void *)&socketAvailable},
        {L"socketClose0:(Z)V" ,(void *)&socketClose0},
        {L"socketShutdown:(I)V" ,(void *)&socketShutdown},
        {L"initProto:()V" ,(void *)&initProto},
        {L"socketSetOption0:(IZ" OBJ ")V" ,(void *)&socketSetOption0},
        {L"socketGetOption:(I" OBJ ")I" ,(void *)&socketGetOption},
        {L"socketSendUrgentData:(I)V" ,(void *)&socketSendUrgentData}
};

/**
 * from java.net.InetAddress
 */
static int IPv4 = 1;
static int IPv6 = 2;

static void  socketCreate(list<Oop *> & _stack) {
    InstanceOop *_this = (InstanceOop *)_stack.front();	_stack.pop_front();
    IntOop *isServer = (IntOop *)_stack.front();	_stack.pop_front();

    int  sockfd ;
    if(isServer->value==1)
        sockfd = socket(AF_INET,SOCK_STREAM,0);
    else
        sockfd = socket(AF_INET,SOCK_DGRAM,0);

    Oop *oop;
    // get the unix fd.
    _this->get_field_value(L"java/net/SocketImpl:fd:Ljava/io/FileDescriptor;", &oop);
    ((InstanceOop *)oop)->set_field_value(L"java/io/FileDescriptor:fd:I",new IntOop(sockfd));

}

static void socketConnect(list<Oop *> & _stack){
    InstanceOop *_this = (InstanceOop *)_stack.front();	_stack.pop_front();
    InstanceOop *address = (InstanceOop *)_stack.front();	_stack.pop_front();
    IntOop *port = (IntOop *)_stack.front();	_stack.pop_front();
    IntOop *timeout = (IntOop *)_stack.front();	_stack.pop_front();

    InstanceOop * holder;

    address->get_field_value(L"java/net/InetAddress:holder:Ljava/net/InetAddress$InetAddressHolder;",reinterpret_cast<Oop **>(&holder));

    IntOop *ip;
    holder->get_field_value(L"java/net/InetAddress$InetAddressHolder:address:I",reinterpret_cast<Oop **>(&ip));

    InstanceOop  *fd ;
    _this->get_field_value(L"java/net/SocketImpl:fd:Ljava/io/FileDescriptor;",reinterpret_cast<Oop **>(&fd));

    IntOop *sockfd;
    fd->get_field_value(L"java/io/FileDescriptor:fd:I",reinterpret_cast<Oop **>(&sockfd));

    struct sockaddr_in server_addr;
    socklen_t socklen = sizeof(server_addr);

    IntOop *family;
    holder->get_field_value(L"java/net/InetAddress$InetAddressHolder:family:I" ,reinterpret_cast<Oop **>(&family));


    if(family->value == IPv4) {
        //ipv4
        bzero(&server_addr, sizeof(server_addr));
        server_addr.sin_addr.s_addr = htonl(ip->value);
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port->value);

        char buf[64];

        std::wstring addr_str = utf8_to_wstring(inet_ntop(AF_INET,&(server_addr.sin_addr.s_addr),buf,sizeof(buf)));
#ifdef  DEBUG
        std::wcout<<L"connect:" <<std::endl;
#endif
        int ret = connect(sockfd->value, (struct sockaddr *) &server_addr, socklen);
        if (ret == 0) {
            //success
#ifdef  DEBUG
    std::wcout<<L"connect success"<<std::endl;
#endif
        }else{
            auto excp_klass = ((InstanceKlass *)BootStrapClassLoader::get_bootstrap().loadClass(L"java/net/SocketException"));
            // get current thread
            vm_thread *thread = (vm_thread *)_stack.back();	_stack.pop_back();
            std::wstring msg = L"connect error:"+ utf8_to_wstring(strerror(errno));
            native_throw_Exception(excp_klass, thread, _stack, msg);
            return;
        }

    }else if(family->value == IPv6){
        //ipv6
        //not support now.
        auto excp_klass = ((InstanceKlass *)BootStrapClassLoader::get_bootstrap().loadClass(L"java/net/SocketException"));
        // get current thread
        vm_thread *thread = (vm_thread *)_stack.back();	_stack.pop_back();
        std::wstring msg = L"connect error , not supported";
        native_throw_Exception(excp_klass, thread, _stack, msg);
    }
}


static void socketBind(list<Oop *> & _stack){
    InstanceOop *_this = (InstanceOop *)_stack.front();	_stack.pop_front();
    InstanceOop *address = (InstanceOop *)_stack.front();	_stack.pop_front();
    IntOop *port = (IntOop *)_stack.front();	_stack.pop_front();

    InstanceOop * holder;
    address->get_field_value(L"java/net/InetAddress:holder:Ljava/net/InetAddress$InetAddressHolder;", reinterpret_cast<Oop **>(&holder));

    IntOop *ip;
    holder->get_field_value(L"java/net/InetAddress$InetAddressHolder:address:I" ,reinterpret_cast<Oop **>(&ip));

    InstanceOop  *fd ;
    _this->get_field_value(L"java/net/SocketImpl:fd:Ljava/io/FileDescriptor;",reinterpret_cast<Oop **>(&fd));

    IntOop *sockfd;
    fd->get_field_value(L"java/io/FileDescriptor:fd:I",reinterpret_cast<Oop **>(&sockfd));

    struct sockaddr_in server_addr;
    socklen_t socklen = sizeof(server_addr);

    IntOop *family;
    holder->get_field_value(L"java/net/InetAddress$InetAddressHolder:family:I" ,reinterpret_cast<Oop **>(&family));


    if(family->value == 1) {
        //ipv4
        bzero(&server_addr, sizeof(server_addr));
        server_addr.sin_addr.s_addr = htonl(ip->value);
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port->value);

        int ret = bind(sockfd->value, (struct sockaddr *) &server_addr, socklen);
        if (ret == 0) {
            //success
#ifdef  DEBUG
            std::wcout<<L"connect success"<<std::endl;
#endif
        }else{
            //exception
            //native_throw_Exception(excp_klass, thread, _stack, msg);
            auto excp_klass = ((InstanceKlass *)BootStrapClassLoader::get_bootstrap().loadClass(L"java/net/SocketException"));
            // get current thread
            vm_thread *thread = (vm_thread *)_stack.back();	_stack.pop_back();
            std::wstring msg = L"bind error:"+ utf8_to_wstring(strerror(errno));
            native_throw_Exception(excp_klass, thread, _stack, msg);
            return;
        }

    }else if(family->value == 2){
        //ipv6
        //not support now.
    }
}

static void socketListen(list<Oop *> & _stack){
    InstanceOop *_this = (InstanceOop *)_stack.front();	_stack.pop_front();
    IntOop *count = (IntOop *)_stack.front();	_stack.pop_front();

    InstanceOop  *fd ;
    _this->get_field_value(L"java/net/SocketImpl:fd:Ljava/io/FileDescriptor;",reinterpret_cast<Oop **>(&fd));

    IntOop *sockfd;
    fd->get_field_value(L"java/io/FileDescriptor:fd:I",reinterpret_cast<Oop **>(&sockfd));

    int ret = listen(sockfd->value,count->value);
    if(ret<0){
        //exception
        auto excp_klass = ((InstanceKlass *)BootStrapClassLoader::get_bootstrap().loadClass(L"java/net/SocketException"));
        // get current thread
        vm_thread *thread = (vm_thread *)_stack.back();	_stack.pop_back();
        std::wstring msg = L"listen error"+ utf8_to_wstring(strerror(errno));
        native_throw_Exception(excp_klass, thread, _stack, msg);
    }

}

static void socketAccept(list<Oop *> & _stack){

    InstanceOop *_this = (InstanceOop *)_stack.front();	_stack.pop_front();
    InstanceOop *clientsocket = (InstanceOop *)_stack.front();	_stack.pop_front();

    vm_thread *thread = reinterpret_cast<vm_thread *>(_stack.back()); _stack.pop_back();


    InstanceOop  *fd ;
    _this->get_field_value(L"java/net/SocketImpl:fd:Ljava/io/FileDescriptor;",reinterpret_cast<Oop **>(&fd));

    IntOop *sockfd;
    fd->get_field_value(L"java/io/FileDescriptor:fd:I",reinterpret_cast<Oop **>(&sockfd));


    struct sockaddr_un peer_addr;
    socklen_t peer_addr_size = sizeof(struct sockaddr_un);
    int clientSockfd  = accept(sockfd->value,(struct sockaddr *) &peer_addr,
            &peer_addr_size);

    //set address

    InstanceOop  *clientfd ;
    clientsocket->get_field_value(L"java/net/SocketImpl:fd:Ljava/io/FileDescriptor;",reinterpret_cast<Oop **>(&clientfd));
    clientfd->set_field_value(L"java/io/FileDescriptor:fd:I",new IntOop(clientSockfd));

    if(peer_addr.sun_family==AF_INET){

        struct sockaddr_in *client_address = reinterpret_cast<sockaddr_in *>(&peer_addr);

        auto inetAddress = ((InstanceKlass *)BootStrapClassLoader::get_bootstrap().loadClass(L"java/net/InetAddress"));

        char buf[128];
        utf8_to_wstring(inet_ntop(AF_INET,&(client_address->sin_addr.s_addr),buf,sizeof(buf)));

        Oop * hostname = java_lang_string::intern_to_oop(utf8_to_wstring(buf));

        ArrayOop * byteArr = static_cast<ArrayKlass *>(BootStrapClassLoader::get_bootstrap().loadClass(L"[B"))->new_instance(4);
        (*byteArr)[0] = new IntOop((0x000000ff  &  client_address->sin_addr.s_addr));
        (*byteArr)[1] = new IntOop((0x0000ff00  &  client_address->sin_addr.s_addr) >> 8);
        (*byteArr)[2] = new IntOop((0x00ff0000  &  client_address->sin_addr.s_addr) >> 16);
        (*byteArr)[3] = new IntOop((0xff000000  &  client_address->sin_addr.s_addr) >> 24);

        Oop *innetAddress = thread->add_frame_and_execute(inetAddress->get_this_class_method(L"getByAddress:(" STR "[B)Ljava/net/InetAddress;"), {hostname,byteArr});

        clientsocket->set_field_value(L"java/net/SocketImpl:address:Ljava/net/InetAddress;",innetAddress);

    }else{
        //not supported now
    }
}

static void socketAvailable(list<Oop *> & _stack){

    InstanceOop *_this = (InstanceOop *)_stack.front();	_stack.pop_front();

    InstanceOop  *fd ;
    _this->get_field_value(L"java/net/SocketImpl:fd:Ljava/io/FileDescriptor;",reinterpret_cast<Oop **>(&fd));

    IntOop *sockfd;
    fd->get_field_value(L"java/io/FileDescriptor:fd:I",reinterpret_cast<Oop **>(&sockfd));

    int bytesAv;
    ioctl(sockfd->value,FIONREAD,&bytesAv);
    _stack.push_back(new IntOop(bytesAv));
}

static void socketClose0(list<Oop *> & _stack){
    InstanceOop *_this = (InstanceOop *)_stack.front();	_stack.pop_front();
    IntOop * useDeferredClose = static_cast<IntOop *>(_stack.front());	_stack.pop_front();

    InstanceOop  *fd ;
    _this->get_field_value(L"java/net/SocketImpl:fd:Ljava/io/FileDescriptor;",reinterpret_cast<Oop **>(&fd));

    IntOop *sockfd;
    fd->get_field_value(L"java/io/FileDescriptor:fd:I",reinterpret_cast<Oop **>(&sockfd));

    int oldfd = sockfd->value;

    if(useDeferredClose->value==1){
        sockfd->value= dup(oldfd);
    }

    int ret = close(oldfd);
    if(ret<0){
        auto excp_klass = ((InstanceKlass *)BootStrapClassLoader::get_bootstrap().loadClass(L"java/net/SocketException"));
        // get current thread
        vm_thread *thread = (vm_thread *)_stack.back();	_stack.pop_back();
        std::wstring msg = L"close error:"+ utf8_to_wstring(strerror(errno));
        native_throw_Exception(excp_klass, thread, _stack, msg);
        return;
    }

}

static void socketShutdown(list<Oop *> & _stack){
    InstanceOop *_this = (InstanceOop *)_stack.front();	_stack.pop_front();
    IntOop * howTo = static_cast<IntOop *>(_stack.front());	_stack.pop_front();

    InstanceOop  *fd ;
    _this->get_field_value(L"java/net/SocketImpl:fd:Ljava/io/FileDescriptor;",reinterpret_cast<Oop **>(&fd));

    IntOop *sockfd;
    fd->get_field_value(L"java/io/FileDescriptor:fd:I",reinterpret_cast<Oop **>(&sockfd));

    int ret = shutdown(sockfd->value,howTo->value);
    if(ret<0){
        auto excp_klass = ((InstanceKlass *)BootStrapClassLoader::get_bootstrap().loadClass(L"java/net/SocketException"));
        // get current thread
        vm_thread *thread = (vm_thread *)_stack.back();	_stack.pop_back();
        std::wstring msg = L"shutdown error"+ utf8_to_wstring(strerror(errno));
        native_throw_Exception(excp_klass, thread, _stack, msg);
    }
}

static  void initProto(list<Oop *> & _stack){

}

static void socketSetOption0(list<Oop *> & _stack){

}

static void socketGetOption(list<Oop *> & _stack){

}

static void socketSendUrgentData(list<Oop *> & _stack){

}

void *java_net_PlainSocketImpl_search_method(const wstring & signature){
    auto iter = methods.find(signature);
    if (iter != methods.end()) {
        return (*iter).second;
    }
    return nullptr;
}
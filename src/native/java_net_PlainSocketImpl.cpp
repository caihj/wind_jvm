//
// Created by fighter on 18-7-8.
//

#include <native/java_lang_Throwable.hpp>
#include "java_net_PlainSocketImpl.hpp"
#include "native/native.hpp"
#include "wind_jvm.hpp"

static unordered_map<wstring, void*> methods = {
        {L"socketCreate:(I)" TRB,(void *)&socketCreate},
        {L"socketConnect:()I",(void *)&socketConnect},
        {L"socketBind:(I)" STE,(void *)&socketBind},
        {L"socketListen:(I)" STE,(void *)&socketListen},
        {L"socketAccept:(I)" STE,(void *)&socketAccept},
        {L"socketAvailable:(I)" STE,(void *)&socketAvailable},
        {L"socketClose0:(I)" STE,(void *)&socketClose0},
        {L"socketShutdown:(I)" STE,(void *)&socketShutdown},
        {L"initProto:(I)" STE,(void *)&initProto},
        {L"socketSetOption0:(I)" STE,(void *)&socketSetOption0},
        {L"socketGetOption:(I)" STE,(void *)&socketGetOption},
        {L"socketSendUrgentData:(I)" STE,(void *)&socketSendUrgentData}
};

static void  socketCreate(list<Oop *> & _stack) {

}

static void socketConnect(list<Oop *> & _stack){

}


static void socketBind(list<Oop *> & _stack){

}

static void socketListen(list<Oop *> & _stack){

}

static void socketAccept(list<Oop *> & _stack){

}

static void socketAvailable(list<Oop *> & _stack){

}

static void socketClose0(list<Oop *> & _stack){

}

static void socketShutdown(list<Oop *> & _stack){

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

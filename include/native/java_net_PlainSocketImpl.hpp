//
// Created by fighter on 18-7-8.
//

#ifndef WIND_JVM_JAVA_NET_PLAINSOCKETIMPL_H
#define WIND_JVM_JAVA_NET_PLAINSOCKETIMPL_H

#include <list>
#include <string>
class Oop;
using std::wstring;
using std::list;


static void  socketCreate(list<Oop *> & _stack) ;

static void socketConnect(list<Oop *> & _stack);

static void socketBind(list<Oop *> & _stack);

static void socketListen(list<Oop *> & _stack);

static void socketAccept(list<Oop *> & _stack);

static void socketAvailable(list<Oop *> & _stack);

static void socketClose0(list<Oop *> & _stack);

static void socketShutdown(list<Oop *> & _stack);

static  void initProto(list<Oop *> & _stack);

static void socketSetOption0(list<Oop *> & _stack);

static void socketGetOption(list<Oop *> & _stack);

static void socketSendUrgentData(list<Oop *> & _stack);

void *java_net_PlainSocketImpl_search_method(const wstring & signature);


#endif //WIND_JVM_JAVA_NET_PLAINSOCKETIMPL_H

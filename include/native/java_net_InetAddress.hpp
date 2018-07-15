//
// Created by fighter on 18-7-15.
//

#ifndef WIND_JVM_JAVA_NET_INETADDRESS_HPP
#define WIND_JVM_JAVA_NET_INETADDRESS_HPP



#include <list>
#include <string>
class Oop;
using std::wstring;
using std::list;

void init(list<Oop *> & _stack);
void isIPv6Supported(list<Oop *> & _stack);
void *java_net_InetAddress_search_method(const wstring & signature);

#endif //WIND_JVM_JAVA_NET_INETADDRESS_HPP

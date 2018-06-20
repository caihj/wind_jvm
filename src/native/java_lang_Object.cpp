/*
 * java_lang_Object.cpp
 *
 *  Created on: 2017年11月19日
 *      Author: zhengxiaolin
 */

#include "native/java_lang_Object.hpp"
#include <vector>
#include <algorithm>
#include <cassert>
#include <system_directory.hpp>
#include <boost/bind.hpp>
#include "native/native.hpp"
#include "runtime/klass.hpp"
#include <boost/bind.hpp>
#include <list>

using std::list;

static unordered_map<wstring, void*> methods = {
	//{L"registerNatives:()",         (void *)&JVM_RegisterNatives},
    {L"hashCode:()I",				(void *)&JVM_IHashCode},
    {L"wait:(J)V",					(void *)&JVM_MonitorWait},
    {L"notify:()V",					(void *)&JVM_MonitorNotify},
    {L"notifyAll:()V",				(void *)&JVM_MonitorNotifyAll},
    {L"clone:()" OBJ,				(void *)&JVM_Clone},
    {L"getClass:()" CLS,				(void *)&Java_java_lang_object_getClass},		// I add one line here.

};

void JVM_RegisterNatives(list<Oop *> & _stack){

	for(auto kv:system_classmap){
		Klass * kclass = kv.second;
		InstanceKlass *instanceKlass = dynamic_cast<InstanceKlass *>(kclass);
		if(instanceKlass!=nullptr){
			for(auto ite:instanceKlass->get_declared_methods()){
				Method * method = ite.second;
				wstring name = instanceKlass->get_name() + L":" + method->get_name() + L":" + method->get_descriptor();
				if(method->is_native() && name != L"java/lang/Object:registerNatives:()V"){
					void * nativeMethod = find_native(instanceKlass->get_name(),method->get_name() + L":" + method->get_descriptor());
					if(nativeMethod == nullptr){
						std::wcerr<<" native not found :"<<instanceKlass->get_name() + L":" + method->get_name() + L":" + method->get_descriptor();
						std::wcerr<<std::endl;
						assert(false);
					}
					method->setNative_method(boost::bind((void (*)(list<Oop *> &))nativeMethod,_1));
				}
			}
		}
	}

#ifdef OBJECT_DEUBG
	std::wcout<<"JVM_RegisterNatives()"<<std::endl;
#endif
}

void JVM_IHashCode(list<Oop *> & _stack){
	InstanceOop *_this = (InstanceOop *)_stack.front();	_stack.pop_front();
	// hash code: I only use address for it.	// in HotSpot `synchronizer.cpp::get_next_hash()`, condition `hashCode = 4`.
	_stack.push_back(new IntOop((intptr_t)_this));
}
void JVM_MonitorWait(list<Oop *> & _stack){
	InstanceOop *_this = (InstanceOop *)_stack.front();	_stack.pop_front();
	long val = ((LongOop *)_stack.front())->value;	_stack.pop_front();
	// wait!!
	_this->wait(val);
}
void JVM_MonitorNotify(list<Oop *> & _stack){
	InstanceOop *_this = (InstanceOop *)_stack.front();	_stack.pop_front();
	assert(false);
}
void JVM_MonitorNotifyAll(list<Oop *> & _stack){
	InstanceOop *_this = (InstanceOop *)_stack.front();	_stack.pop_front();
	_this->notify_all();
}
void JVM_Clone(list<Oop *> & _stack){
	Oop *_this = _stack.front();	_stack.pop_front();

	if (_this->get_klass()->get_type() == ClassType::InstanceClass) {
		// then, it must implemented the Ljava/lang/Cloneable; interface...
		auto instance_klass = ((InstanceKlass *)_this->get_klass());
		if (!instance_klass->check_interfaces(L"java/lang/Cloneable")) {
			std::wcerr << _this->get_klass()->get_name() << " doesn't implemented the java/lang/Cloneable interface ??" << std::endl;
			assert(false);
		}

		// shallow copy
		InstanceOop *clone = new InstanceOop(*((InstanceOop *)_this));
		_stack.push_back(clone);
#ifdef DEBUG
	sync_wcout{} << "(DEBUG) cloned from obj [" << _this << "] (InstanceOop) to new cloned obj [" << clone << "]." << std::endl;
#endif

	} else if (_this->get_klass()->get_type() == ClassType::TypeArrayClass || _this->get_klass()->get_type() == ClassType::ObjArrayClass) {
		// default implemented java/lang/Cloneable IMPLICITLY. should not check.
		auto array_klass = ((ArrayKlass *)_this->get_klass());

		// shallow copy
		ArrayOop *clone = new ArrayOop(*((ArrayOop *)_this));
		_stack.push_back(clone);

#ifdef DEBUG
	sync_wcout{} << "(DEBUG) cloned from obj [" << _this << "] (ArrayOop) to new cloned obj [" << clone << "]." << std::endl;
#endif
	} else {
		assert(false);
	}
}
void Java_java_lang_object_getClass(list<Oop *> & _stack){
	InstanceOop *_this = (InstanceOop *)_stack.front();	_stack.pop_front();
	assert(_this != nullptr);
	_stack.push_back(_this->get_klass()->get_mirror());
}

void *java_lang_object_search_method(const wstring & signature)
{
	auto iter = methods.find(signature);
	if (iter != methods.end()) {
		return (*iter).second;
	}
	return nullptr;
}

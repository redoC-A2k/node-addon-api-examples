#include "someObject.hpp"

#include <napi.h>

SomeObject::SomeObject(const Napi::CallbackInfo &info) : Napi::ObjectWrap<SomeObject>(info) {
    this->_value_ = info[0].As<Napi::Number>().DoubleValue();
}

void SomeObject::Init(Napi::Env env, Napi::Object exports) {
	Napi::Function func = DefineClass(env, "SomeObject", {});
	Napi::FunctionReference *constructor = new Napi::FunctionReference();
	*constructor = Napi::Persistent(func);
	env.SetInstanceData(constructor);
	exports.Set("SomeObject", func);
}

Napi::Object SomeObject::NewInstance(Napi::Env env, Napi::Value arg) {
	Napi::Object obj = env.GetInstanceData<Napi::FunctionReference>()->New({arg});
	return obj;
}
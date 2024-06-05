#include "object.hpp"
#include <napi.h>

Napi::Object Object::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "SomeObject", {InstanceMethod("plusOne", &Object::PlusOne)});
    Napi::FunctionReference *constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);
    exports.Set("SomeObject", func);
    return exports;
}

Object::Object(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Object>(info)
{
    this->_counter_ = info[0].As<Napi::Number>().DoubleValue();
}

Napi::Object Object::NewInstance(Napi::Env env, Napi::Value arg)
{
    Napi::EscapableHandleScope scope(env);
    Napi::Object obj = env.GetInstanceData<Napi::FunctionReference>()->New({arg});
    return scope.Escape(napi_value(obj)).ToObject();
}

Napi::Value Object::PlusOne(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    this->_counter_ = this->_counter_ + 1;
    return Napi::Number::New(env, this->_counter_);
}
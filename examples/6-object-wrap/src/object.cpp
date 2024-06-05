#include "object.hpp"

Napi::Object Object::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Object", {
                                                         InstanceMethod("getValue", &Object::GetValue),
                                                         InstanceMethod("plusOne", &Object::PlusOne),
                                                         InstanceMethod("multiply", &Object::Multiply),
                                                     });

    Napi::FunctionReference *constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);
    exports.Set("Object", func);
    return exports;
}

Object::Object(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Object>(info)
{
    Napi::Env env = info.Env();
    int length = info.Length();
    if (length <= 0 || !info[0].IsNumber())
    {
        Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
        return;
    }

    Napi::Number value = info[0].As<Napi::Number>();
    this->_value_ = value.DoubleValue();
}

Napi::Value Object::GetValue(const Napi::CallbackInfo &info)
{
    double num = this->_value_;
    return Napi::Number::New(info.Env(), num);
}

Napi::Value Object::PlusOne(const Napi::CallbackInfo &info)
{
    this->_value_ = this->_value_ + 1;
    return Object::GetValue(info);
}

Napi::Value Object::Multiply(const Napi::CallbackInfo &info)
{
    Napi::Number multiple;
    if (info.Length() <= 0 || !info[0].IsNumber())
    {
        multiple = Napi::Number::New(info.Env(), 1);
    }
    else
    {
        multiple = info[0].As<Napi::Number>();
    }

    Napi::Object obj = info.Env().GetInstanceData<Napi::FunctionReference>()->New({Napi::Number::New(info.Env(), this->_value_ * multiple.DoubleValue())});
    return obj;
}


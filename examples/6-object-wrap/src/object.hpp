#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <napi.h>

class Object : public Napi::ObjectWrap<Object>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    Object(const Napi::CallbackInfo &info);

private:
    Napi::Value GetValue(const Napi::CallbackInfo &info);
    Napi::Value PlusOne(const Napi::CallbackInfo &info);
    Napi::Value Multiply(const Napi::CallbackInfo &info);

    double _value_;
};

#endif
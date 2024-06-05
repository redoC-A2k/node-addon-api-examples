#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <napi.h>

class Object : public Napi::ObjectWrap<Object>
{
private:
    Napi::Value PlusOne(const Napi::CallbackInfo &info);
    double _counter_;

public:
    Object(const Napi::CallbackInfo &info);
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object NewInstance(Napi::Env env, Napi::Value arg);
};

#endif
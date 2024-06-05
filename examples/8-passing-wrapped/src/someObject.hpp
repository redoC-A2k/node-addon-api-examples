#ifndef SOMEOBJECT_H
#define SOMEOBJECT_H

#include <napi.h>

class SomeObject : public Napi::ObjectWrap<SomeObject> {
public:
    static void Init(Napi::Env env, Napi::Object exports);
    static Napi::Object NewInstance(Napi::Env env, Napi::Value arg);
    double Val() const { return _value_; }
    SomeObject(const Napi::CallbackInfo &info);

private:
    double _value_;
};

#endif
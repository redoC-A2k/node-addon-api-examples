#include "someObject.hpp"
#include <napi.h>

Napi::Object CreateObject(const Napi::CallbackInfo &info) {
    return SomeObject::NewInstance(info.Env(), info[0]);
}

Napi::Number Add(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    SomeObject *obj1 = Napi::ObjectWrap<SomeObject>::Unwrap(info[0].As<Napi::Object>());
    SomeObject *obj2 = Napi::ObjectWrap<SomeObject>::Unwrap(info[1].As<Napi::Object>());
    double sum = obj1->Val() + obj2->Val();
    return Napi::Number::New(env, sum);
}


Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    SomeObject::Init(env, exports);
    exports.Set(Napi::String::New(env, "CreateObject"), Napi::Function::New(env, CreateObject));
    exports.Set(Napi::String::New(env, "Add"), Napi::Function::New(env, Add));
    return exports;
}

NODE_API_MODULE(addon, InitAll)
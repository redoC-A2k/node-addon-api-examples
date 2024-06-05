#include <napi.h>
#include "object.hpp"

Napi::Object createObject(const Napi::CallbackInfo &info)
{
    return Object::NewInstance(info.Env(), info[0]);
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
    Napi::Object new_exports = Napi::Function::New(env, createObject, "CreateObject");
    return Object::Init(env, new_exports);
}

NODE_API_MODULE(addon, InitAll)
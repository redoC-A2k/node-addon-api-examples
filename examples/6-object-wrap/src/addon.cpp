#include <napi.h>
#include "object.hpp"

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
    return Object::Init(env, exports);
}

NODE_API_MODULE(objectWrapAddon, InitAll)
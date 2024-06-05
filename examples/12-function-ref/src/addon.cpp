#include <napi.h>
#include "native.hpp"

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    NativeAddon::Init(env, exports);
    return exports;
}

NODE_API_MODULE(addon, Init)
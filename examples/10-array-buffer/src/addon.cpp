#include <cstdio>
#include <napi.h>

static Napi::Value AcceptByteArray(const Napi::CallbackInfo &info) {
    if (info.Length() != 1) {
        Napi::Error::New(info.Env(), "Expected exactly one argument")
            .ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }

    if (!info[0].IsTypedArray()) {
        Napi::Error::New(info.Env(), "Expected a TypedArray")
            .ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }
    Napi::TypedArray typedArray = info[0].As<Napi::TypedArray>();

    if (typedArray.TypedArrayType() != napi_uint8_array) {
        Napi::Error::New(info.Env(), "Expected an Uint8Array")
            .ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }
    Napi::Uint8Array uint8Array = typedArray.As<Napi::Uint8Array>();

    // Copy to std::vector<uint8_t>:
    std::vector<uint8_t> bytes(uint8Array.Data(),
                               uint8Array.Data() + uint8Array.ElementLength());
    printf("std::vector<uint8_t> from Uint8Array: [");
    for (uint8_t byte : bytes) {
        printf("%d, ", byte);
    }
    printf("\b\b]\n");

    return info.Env().Undefined();
}

static Napi::Value CreateByteArray(const Napi::CallbackInfo &info) {
    if (info.Length() != 1) {
        Napi::Error::New(info.Env(), "Expected exactly one argument")
            .ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }

    if (!info[0].IsArray()) {
        Napi::Error::New(info.Env(), "Expected an Array")
            .ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }

    Napi::Array array = info[0].As<Napi::Array>();
    size_t arrayLength = array.Length();

    // Create std::vector<uint8_t> out of array.
    // We allocate it on the heap to allow wrapping it up into ArrayBuffer.
    std::unique_ptr<std::vector<uint8_t>> nativeArray =
        std::make_unique<std::vector<uint8_t>>(arrayLength, 0);

    for (size_t i = 0; i < arrayLength; i++) {
        Napi::Value item = array[i];
        if (!item.IsNumber()) {
            Napi::Error::New(info.Env(), "Expected an Array of numbers")
                .ThrowAsJavaScriptException();
            return info.Env().Undefined();
        }
        Napi::Number num = item.As<Napi::Number>();
        double numValue = num.DoubleValue();
        if (numValue < 0 || numValue > 255) {
            Napi::Error::New(info.Env(), "Expected an Array of numbers in range [0, 255]")
                .ThrowAsJavaScriptException();
            return info.Env().Undefined();
        }
        (*nativeArray)[i] = static_cast<uint8_t>(numValue);
    }

    printf("std vector<uint8_t> from Array: [");
    for (uint8_t byte : *nativeArray) {
        printf("%d, ", byte);
    }
    printf("\\b\\b]\n");

    Napi::ArrayBuffer arrayBuffer = Napi::ArrayBuffer::New(info.Env(), nativeArray->data(), nativeArray->size());
    return arrayBuffer;
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("AcceptByteArray", Napi::Function::New(env, AcceptByteArray));
    exports.Set("CreateByteArray", Napi::Function::New(env, CreateByteArray));
    return exports;
}

NODE_API_MODULE(addon, Init)
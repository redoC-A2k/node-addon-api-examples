#include <napi.h>

Napi::Value Add(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 2)
    {
        Napi::TypeError::New(info.Env(), "Wrong number of arguments")
            .ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }

    if (!info[0].IsNumber() || !info[1].IsNumber())
    {
        Napi::TypeError::New(env, "wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    double a = info[0].As<Napi::Number>().DoubleValue();
    double b = info[1].As<Napi::Number>().DoubleValue();
    Napi::Number num = Napi::Number::New(env, a + b);

    return num;
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "add"), Napi::Function::New(env, Add));
    return exports;
}

NODE_API_MODULE(funcAddon, Init);
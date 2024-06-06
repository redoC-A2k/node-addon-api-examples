#include <memory>
#include <napi.h>

class AsyncIteratorExample : public Napi::ObjectWrap<AsyncIteratorExample> {
private:
    int _from;
    int _to;

public:
    AsyncIteratorExample(const Napi::CallbackInfo &info) : Napi::ObjectWrap<AsyncIteratorExample>(info),
                                                           _from(info[0].As<Napi::Number>()),
                                                           _to(info[1].As<Napi::Number>()) {}

    static Napi::Object Init(Napi::Env env, Napi::Object exports) {
        Napi::Function func = DefineClass(env,
                                          "AsyncIterator",
                                          {InstanceMethod(Napi::Symbol::WellKnown(env, "asyncIterator"),
                                                          &AsyncIteratorExample::Iterator)});
        exports.Set("AsyncIterator", func);
        return exports;
    }

    Napi::Value Iterator(const Napi::CallbackInfo &info) {
        auto env = info.Env();
        auto iteratorObj = Napi::Object::New(env);
        iteratorObj["current"] = Napi::Number::New(env, _from);
        iteratorObj["last"] = Napi::Number::New(env, _to);

        auto next = Napi::Function::New(env, [](const Napi::CallbackInfo &info) {
            auto env = info.Env();
            auto deferred = std::make_shared<Napi::Promise::Deferred>(Napi::Promise::Deferred::New(env));
            auto iteratorObj = info.This().As<Napi::Object>();

            auto callback = Napi::Function::New(env, [=](const Napi::CallbackInfo &info) {
                auto env = info.Env();
                auto value = Napi::Object::New(env);
                auto iteratorObject = info.This().As<Napi::Object>();

                auto current = iteratorObject.Get("current").As<Napi::Number>().Int32Value();
                auto last = iteratorObject.Get("last").As<Napi::Number>().Int32Value();
                auto done = current > last;

                if(done){
                    value["done"] = Napi::Boolean::New(env, true);
                } else {
                    value["done"] = Napi::Boolean::New(env, false);
                    value["value"] = Napi::Number::New(env, current);
                    iteratorObject.Set("current", Napi::Number::New(env, current + 1));
                }

                deferred->Resolve(value); }, "next");

            env.Global().Get("setTimeout").As<Napi::Function>().Call({callback.Get("bind").As<Napi::Function>().Call(callback, {iteratorObj}), Napi::Number::New(env, 1000)});

            return deferred->Promise();
        });

        iteratorObj["next"] = next.Get("bind").As<Napi::Function>().Call(next, {iteratorObj});

        return iteratorObj;
    }
};

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    AsyncIteratorExample::Init(env, exports);
    return exports;
}

NODE_API_MODULE(addon, Init)
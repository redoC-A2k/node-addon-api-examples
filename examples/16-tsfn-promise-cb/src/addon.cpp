#include <future>
#include <iostream>
#include <napi.h>
#include <thread>

class PromiseCallbackDemo;
using Context = PromiseCallbackDemo;
using DataType = std::promise<std::string>;
void CallJs(Napi::Env env, Napi::Function callback, Context *context, DataType *data);
using TSFN = Napi::TypedThreadSafeFunction<Context, DataType, CallJs>;
using FinalizerDataType = void;
void thread_entry(PromiseCallbackDemo *obj);

class PromiseCallbackDemo : public Napi::ObjectWrap<PromiseCallbackDemo> {
public:
    TSFN tsfn;
    std::thread native_thread;
    static Napi::Function GetClass(Napi::Env env);

    PromiseCallbackDemo(const Napi::CallbackInfo &info) : ObjectWrap(info) {
        Napi::Env env = info.Env();
        // Argument checking
        if (info.Length() < 1) {
            Napi::TypeError::New(env, "Wrong number of arguments")
                .ThrowAsJavaScriptException();
            return;
        }

        if (!info[0].IsFunction()) {
            Napi::TypeError::New(env, "First argument must be a function")
                .ThrowAsJavaScriptException();
            return;
        }

        Ref();

        tsfn = TSFN::New(
            env,
            info[0].As<Napi::Function>(),
            "PromiseCallbackDemo",
            0,
            1,
            this,
            [](Napi::Env env,
               FinalizerDataType *,
               Context *ctx) {
                ctx->Unref();
                ctx->native_thread.join();
            });

        native_thread = std::thread(thread_entry, this);
    }
};

void thread_entry(PromiseCallbackDemo *obj) {
    for (auto i = 0; i < 10; ++i) {
        std::promise<std::string> promise;
        auto future = promise.get_future();
        obj->tsfn.NonBlockingCall(&promise);
        try {
            auto result = future.get();
            std::cout << "Result from JS, call " << i << ": " << result << std::endl;
        } catch (const std::exception &e) {
            std::cout << "Error from JS, call " << i << ": " << e.what() << std::endl;
        }
    }
    obj->tsfn.Release();
}

void CallJs(Napi::Env env, Napi::Function callback, Context *context, DataType *data) {
    if (env != nullptr) {
        auto jsPromise = callback.Call({}).As<Napi::Object>();
        auto then = jsPromise.Get("then").As<Napi::Function>();

        then.Call(jsPromise, {Napi::Function::New(env, [=](const Napi::CallbackInfo &info) {
                                  auto result = info[0].As<Napi::String>().Utf8Value();
                                  data->set_value(result);
                              }),
                              Napi::Function::New(env, [=](const Napi::CallbackInfo &info) {
                                  auto result = info[0].As<Napi::String>().Utf8Value();
                                  data->set_exception(std::make_exception_ptr(std::runtime_error(result)));
                              })});
    } else {
        data->set_exception(std::make_exception_ptr(std::runtime_error("Environment is shutdown")));
    }
}

Napi::Function PromiseCallbackDemo::GetClass(Napi::Env env) {
    return DefineClass(env, "PromiseCallbackDemo", {});
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("PromiseCallbackDemo", PromiseCallbackDemo::GetClass(env));
    return exports;
}

NODE_API_MODULE(addon, Init)
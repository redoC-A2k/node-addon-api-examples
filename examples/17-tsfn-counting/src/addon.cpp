#include <chrono>
#include <napi.h>
#include <thread>

constexpr size_t ARRAY_LENGTH = 10;

struct TsfnContext {
    TsfnContext(Napi::Env env) : deferred(Napi::Promise::Deferred::New(env)) {
        for (size_t i = 0; i < ARRAY_LENGTH; ++i)
            ints[i] = i;
    }

    Napi::Promise::Deferred deferred;
    std::thread nativeThread;
    int ints[ARRAY_LENGTH];
    Napi::ThreadSafeFunction tsfn;
};

void threadEntry(TsfnContext *context);
void FinalizerCallback(Napi::Env env, void *finalizeData, TsfnContext *context);

Napi::Value CreateTSFN(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    auto testData = new TsfnContext(env);

    testData->tsfn = Napi::ThreadSafeFunction::New(
        env,
        info[0].As<Napi::Function>(),
        "TSFN",
        0,
        1,
        testData,
        FinalizerCallback,
        (void *)nullptr);

    testData->nativeThread = std::thread(threadEntry, testData);
    return testData->deferred.Promise();
}

void threadEntry(TsfnContext *context) {
    auto callback = [](Napi::Env env, Napi::Function jsCallback, int *data) {
        jsCallback.Call({Napi::Number::New(env, *data)});
    };

    for (size_t index = 0; index < ARRAY_LENGTH; ++index) {
        napi_status status = context->tsfn.BlockingCall(&context->ints[index], callback);

        if (status != napi_ok) {
            Napi::Error::Fatal(
                "ThreadEntry",
                "Napi::ThreadSafeNapi::Function::BlockingCall() failed");
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    context->tsfn.Release();
}

void FinalizerCallback(Napi::Env env, void *finalizeData, TsfnContext *context) {
    context->nativeThread.join();
    context->deferred.Resolve(Napi::Boolean::New(env, true));
    delete context;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports["createTSFN"] = Napi::Function::New(env, CreateTSFN);
    return exports;
}

NODE_API_MODULE(addon, Init);
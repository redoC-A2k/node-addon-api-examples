#include "SimpleWorker.hpp"

Napi::Value runSimpleAsyncWorker(const Napi::CallbackInfo &info) {
    int runTime = info[0].As<Napi::Number>();
    Napi::Function callback = info[1].As<Napi::Function>();
    SimpleAsyncWorker *worker = new SimpleAsyncWorker(callback, runTime);
    worker->Queue();
    std::string msg = "SimpleAsyncWorker for " + std::to_string(runTime) + " seconds queued.";
    return Napi::String::New(info.Env(), msg.c_str());
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports["runSimpleAsyncWorker"] = Napi::Function::New(env, runSimpleAsyncWorker, std::string("runSimpleAsyncWorker"));
    return exports;
}

NODE_API_MODULE(addon, Init)
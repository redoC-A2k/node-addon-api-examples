#pragma once
#include <napi.h>

class SimpleAsyncWorker : public Napi::AsyncWorker {
private:
    int runTime;

public:
    SimpleAsyncWorker(Napi::Function &callback, int runTime);
    virtual ~SimpleAsyncWorker(){};

    void Execute();
    void OnOK();
};
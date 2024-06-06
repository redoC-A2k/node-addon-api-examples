const { PromiseCallbackDemo } = require("bindings")("TsfnWorkerAddon")

let shouldFail = false;

function jsCallback() {
    return new Promise((resolve, reject) => {
        return setTimeout(() => {
            ((shouldFail = !shouldFail) ? reject : resolve)(new Date().toISOString());
        }, 100);
    });
}

new PromiseCallbackDemo(jsCallback)
const addon = require("bindings")("AsyncWorkerAddon")

let result = addon.runSimpleAsyncWorker(2, AsyncWorkerCompletion);
console.log("runSimpleAsyncWorker returned '"+result+"'.");

result = addon.runSimpleAsyncWorker(4, AsyncWorkerCompletion);
console.log("runSimpleAsyncWorker returned '"+result+"'.");

result = addon.runSimpleAsyncWorker(8, AsyncWorkerCompletion);
console.log("runSimpleAsyncWorker returned '"+result+"'.");

function AsyncWorkerCompletion (err, result) {
    if (err) {
        console.log("SimpleAsyncWorker returned an error: ", err);
    } else {
        console.log("SimpleAsyncWorker returned '"+result+"'.");
    }
};
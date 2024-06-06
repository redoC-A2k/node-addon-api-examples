const addon = require("bindings")("AsyncWorkerAddon")
let result = addon.doHeavyMath(2,1)
result.then(x=>{console.log("data x = ",x)})
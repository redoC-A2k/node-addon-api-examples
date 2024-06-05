const addon = require("bindings")("FunctionFactoryAddon")
let fn = addon();
let result = fn();
console.log(result);
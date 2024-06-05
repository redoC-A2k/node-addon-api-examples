const addon = require("bindings")("PassingWrapAddon")

let obj1 = addon.CreateObject(10);
let obj2 = addon.CreateObject(20);
let result = addon.Add(obj1, obj2);

console.log(result);
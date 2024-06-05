let addon = require("bindings")("ObjectWrapAddon")
let obj = new addon.Object(10);
console.log(obj.plusOne());
console.log(obj.plusOne());
console.log(obj.plusOne());

console.log(obj);
console.log(obj.multiply().getValue());

let obj2 = obj.multiply(-1);
console.log(obj2.getValue());
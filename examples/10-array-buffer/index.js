const addon = require('bindings')('ArrayBufferAddon');

{
    const byteArray = Uint8Array.from([10, 20, 30, 50, 25, 17]);
    console.log("byteArray: ", byteArray);
    addon.AcceptByteArray(byteArray);
}
console.log("---------");

{
    const array = [7, 15, 26, 58, 64];
    console.log("array: ", array);
    const byteArray = addon.CreateByteArray(array);
    console.log("byteArray: ", byteArray);
}
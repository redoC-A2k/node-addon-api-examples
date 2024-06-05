const addon = require("bindings")("ArrayBufferAddon")
const array = new Int32Array(10);

array[0] = 19;
array[1] = -41;
array[2] = 98;
array[3] = -922;
array[4] = 587;
array[5] = 12;
array[6] = 221;
array[7] = 49;
array[8] = -96;
array[9] = -1;

addon.AcceptArrayBuffer(array.buffer);
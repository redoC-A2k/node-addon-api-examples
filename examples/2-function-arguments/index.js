const addon = require('bindings')('funcAddon');

console.log("This should be eight ", addon.add(3, 5));
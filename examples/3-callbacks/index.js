const addon = require('bindings')('callbacksAddon')

addon(function (msg) {
    console.log(msg)
})
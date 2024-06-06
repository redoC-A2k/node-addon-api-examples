const { createTSFN } = require('bindings')('TsfnWorkerAddon');

const callback = (...args) => { 
    console.log(new Date, ...args); 
};

void async function() {
    console.log(await createTSFN(callback));
}();

for(let i=0; i<50; i++)
console.log("hello")
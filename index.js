// print('printing.. Hey dude!')

const setTimeout = (cb, ms) => timeout(ms, 0, cb);
const interval = (cb, ms) => timeout(0, ms, cb);

// const result = setTimeout(function (err, res) {
//   print(`error: ${err}`);
//   print(`res ${res}`);

// }, 1000);
// interval(() => { print('executing...', Date.now())}, 300);
setTimeout(() => {
  print('ran after 1s')
}, 1000);

setTimeout(() => {
  print('ran after 2s')
}, 2000);
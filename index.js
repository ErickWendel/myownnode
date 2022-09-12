print('printing.. Hey dude!')

const setTimeout = (cb, ms) => timeout(ms, 10, cb);
const interval = (cb, ms) => timeout(10, ms, cb);

timeout(100, 0, function () {
  print('ran after 1s')
  timeout(100, 0, function () {
    print('ran after 2s')
  }, 2000);

}, 1000);
print('printing.. 1Hey dude!')
print('printing.. 2Hey dude!')
print('printing.. 3Hey dude!')
print('printing.. 4Hey dude!')

const setTimeout = (ms, cb) => timeout(ms, 0, cb);
const setInterval = (ms, cb) => timeout(0, ms, cb);

setTimeout(100, () => {
  print('1 ' + new Date().toISOString());

  setTimeout(200, () => {
    print('2 ' + new Date().toISOString());
    setTimeout(200, () => print('3 ' + new Date().toISOString()));
  });

});
const map = new Map([
  ['name', 'erick']
])
print([...map.values()].map(i => i.toUpperCase()))

setInterval(500, () => print('in parallel', new Date().toISOString()))
setTimeout(1000, () => print('after all', new Date().toISOString()))
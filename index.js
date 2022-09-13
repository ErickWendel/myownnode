print('printing.. 1Hey dude!')
print('printing.. 2Hey dude!')
print('printing.. 3Hey dude!')
print('printing.. 4Hey dude!')

// const setTimeout = (cb, ms) => timeout(ms, 10, cb);
// const interval = (cb, ms) => timeout(10, ms, cb);


for(let i=0; i< 10; i++) {
  print('ae', i);
}

print( new Date().toISOString(), 'ran')
timeout(1000, 0, function () {
  print( new Date().toISOString(), 'ran after 1s')

  function test() {
    timeout(2000, 0, function () {
      print( new Date().toISOString(), 'ran after 2s')
    });
  }
  test()
});
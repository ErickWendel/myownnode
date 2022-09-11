// print('printing.. Hey dude!')
const result = timeout(100, 0, (err, res) => {
  print (`error: ${err}`);
  print (`res ${res}`);
  1 +1
});
print(`result: ${result}`)
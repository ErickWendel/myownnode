print('printing.. Hey dude!')
// print('printing.. 2Hey dude!')
// print('printing.. 3Hey dude!')
// print('printing.. 4Hey dude!')

const setTimeout = (ms, cb) => timeout(ms, 0, cb);
const setInterval = (ms, cb) => timeout(0, ms, cb);
const setTimeoutAsync = (ms) => new Promise(resolve => setTimeout(ms, resolve))
// setTimeout(100, () => {
//   print('1 ' + new Date().toISOString());

//   setTimeout(200, () => {
//     print('2 ' + new Date().toISOString());
//     setTimeout(200, () => print('3 ' + new Date().toISOString()));
//   });

// });

;
(async function asyncFn() {
  print(new Date().toISOString(), 'waiting a sec...')
  await setTimeoutAsync(1000)
  print(new Date().toISOString(), 'waiting a sec...')
  await setTimeoutAsync(1000)
  print(new Date().toISOString(), 'finished at')
})()

// const map = new Map([
//   ['name', 'erick']
// ])
// print([...map.values()].map(i => i.toUpperCase()))

// setInterval(500, () => print('in parallel', new Date().toISOString()))
// setTimeout(1000, () => print('after all', new Date().toISOString()))

class Db {
  constructor(db = new Set()) {
    this.db = db
  }
  async insert(data) {
    await setTimeoutAsync(200);
    this.db.add(data)
  }
  async removeAll() {
    await setTimeoutAsync(200);
    this.db.clear()
  }
  async findAll() {
    await setTimeoutAsync(200);
    return [...this.db.values()].map(({ name, age }) => `name: ${name}, age: ${age}`)
  }
}

;(async function asyncFn() {

  const db = new Db()
  await db.insert({
    name: 'erick',
    age: 27
  })

  print('all', await db.findAll())

  await db.removeAll({
    name: 'erick',
    age: 27
  })

  print('empty', await db.findAll())
})()

// const promises = []
// for(let i = 0; i< 10000; i++) {
//   p = setTimeoutAsync(i).then(_ => `${Date.now()}-finished-${i}\n`)
//   promises.push(p)
// }

// Promise.all(promises).then(_ => print('results', _))
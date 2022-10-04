let interval = 0;
let sleep = 1000
print('Hello World ' + new Date().toISOString())
// timeout(sleep, interval, () => {
//     print('1 ' + new Date().toISOString())
//     timeout(sleep, interval, () => {
//         print('2 ' + new Date().toISOString())
//         timeout(sleep, interval, () => {
//             print('3 ' + new Date().toISOString())
//         })
//     })
// })

const setTimeout = (ms, cb) => timeout(ms, 0, cb);
const setInterval = (ms, cb) => timeout(0, ms, cb);
const setTimeoutAsync = (ms) =>
    new Promise(resolve => setTimeout(ms, resolve));

; (async function asyncFn() {
    print(new Date().toISOString(), 'waiting a sec....');
    await setTimeoutAsync(1000);
    print(new Date().toISOString(), 'waiting a sec....');
    await setTimeoutAsync(1000);
    print(new Date().toISOString(), 'finished2....');
})();


thread(10);

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
        return [...this.db.values()].map(({
            name,
            age
        }) => `name: ${name}, age: ${age}`)
    }
}

;
(async function asyncFn() {

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
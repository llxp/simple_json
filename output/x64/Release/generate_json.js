const fs = require('fs');
var crypto = require("crypto");

function generate(count)
{
    return crypto.randomBytes(count).toString("hex");
}

function getRandomIntInclusive(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min +1)) + min;
}

function getRandomBool()
{
    var rndBool = Math.random();
    if(rndBool < 0.5) {
        return false;
    } else {
        return true;
    }
}

function generateObj() {
    var obj = {
        _id: '',
        index: '',
        guid: '',
        isActive: false,
        balance: '123',
        picture: 'abc',
        age: 65535,
        eyeColor: '',
        name: {
            first: '',
            last: ''
        },
        bools: [],
        strings: []
    };
    obj._id = generate(16);
    obj.index = generate(10);
    obj.guid = generate(12);
    obj.age = getRandomIntInclusive(0, 100);
    obj.isActive = getRandomBool();
    for(var x = 0;x < 20;x++) {
        obj.bools.push(getRandomBool());
    }
    for(var x = 0;x < 20;x++) {
        obj.strings.push(generate(16));
    }
    return obj;
}

var x = [];
for(var i = 0;i < 1000000;i++) {
    var h = {
        'x': Math.random(),
        'y': Math.random(),
        'z': Math.random(),
        'name': generate(5),
        'opts': { '1': getRandomBool() }
    }
    x.push(h);
}

var rootObj2 = {};
rootObj2["items"] = x;

fs.writeFile("1.json", JSON.stringify(rootObj2, null, 4), (err) => {
    if (err) throw err;
    console.log('The file has been saved!');
});

var rootObj = {};
var key = "items";
rootObj[key] = [];

for(var i = 0;i < 5000;i++) {
    rootObj[key].push(generateObj());
}

var str = JSON.stringify(rootObj, null, 4);

fs.writeFile("abc.txt", str, (err) => {
    if (err) throw err;
    console.log('The file has been saved!');
});
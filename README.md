# Hedgehog

It is hard to design a good programming language.

```
fn fac(n) {
  if n <=1 { return 1 } else { return n * fac(n-1) }
}

# tail call version:
fn fac_tailcall(n) {
  fn f(acc, n) {
    if n <= 1 { return acc} else { return fn(acc*n, n-1) }
  }
  return f(1, n)
}
```
## Modules

```
-- math.
m = {}
m.pi = 3.14159

m.add = fn (x, y) { return x+y }

return m
```

```
math = require("math") -- math is a local number
print = require("std").print

math.add(3, 9) --> 12
print(math.pi) --> 3.14159
```

```
# a modules looks like this for interpreter:
fu () {
    m = {}
    m.pi = 3.14159

    m.add = fn (x, y) { return x+y }

    return m
}()
```


## OOP

```
fn Car(speed) {
  car = {.postion = 0}

  -- methods of a object
  car.run = fn (time) { car.postion += time }
  return car
}

car = Car(4)
car.run(3)
print(car.postion) --> 12
```

```
fn Base() {
  return {.hello: fn () { return "hello from base" }}
}

base = Base()

fn A() {
  a = {}
  setmeta(a, base)
  return a
}

a = A()
a.hello() -- `hello` of base will be called
```

## functional

```
fn Fun(t) {
  m = {}
  setmeta(t, m)
  
  m.map = fn (f) {
    res = {}
    setmeta(res, m)
    for k, v in t {
      res[k] = f(v)
    }
    return res
  }
    
  m.filter = fn (f) {
    res = {}
    setmeta(res, m)
    for k, v in t {
      res[k] = f(v)
    }
    return res
  }
  
  return t
}

Fun({1, 2, 3, 4}).
    filter(>0).
    map(+1)
```

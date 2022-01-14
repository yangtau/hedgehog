# Hedgehog

It is hard to design a good programming language 😭.

```
fn fac(n) {
  if n <=1 { return 1 } else { return n * fac(n-1) }
}

# tail call version:
fn fac_tailcall(n) {
  fn fn(acc, n) {
    if n <= 1 { return acc} else { return fn(acc*n, n-1) }
  }
  return fn(1, n)
}
```
## Modules

```
-- math.
m = {}
m.pi = 3.14159

fn m.add(x, y) { return x+y }

return m
```

```
math = require "math" -- math is a local number

math.add(3, 9) --> 12
print(math.pi) --> 3.14159
```

**global enviroment:**

```
print = G.print -- all modules share the same `G` table
G = {} -- override global locally

greet=fn() {print("hello")}
return {greet: greet}
```

```
# a modules looks like this for interpreter:
fn (G) {
  print = global.print
  global = {}

  greet=fn() {print("hello")}
  return {greet: greet}
}
```


## OOP

```
fn Car(speed) {
  car = {postion = 0}

  -- methods of a object
  fn car.run(time) { car.postion += time }
  return car
}

car = Car(4)
car.run(3)
print(car.postion) --> 12
```

```
fn Base() {
  o = {}
  fn o.hello() { return "hello from base" }
  return o
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



```
fn Greeter(Namer) {
  o = {}
  fn o.greet() { return "hello, " + Namer.name() }
  return o
}

fn Human(name) {
  o = {}
  G.setmeta(o, Greeter(o))
  fn o.name() { return name }
  return o
}

a = Human("yangtao")
a.greet()
```

## functional

```
fn Fun(t) {
  m = {}
  setmeta(t, m)
  
  fn m.map(f) {
    res = {}
    setmeta(res, m)
    for k, v in t {
      res[k] = f(v)
    }
    return res
  }
    
  fn m.filter(f) {
    res = {}
    setmeta(res, m)
    for k, v in t {
      res[k] = f(v)
    }
    return res
  }
  
  return t
}

-- use `->` to call function of meta table (make a table pure data)
Fun({1, 2, 3, 4})->
    filter(>0)->
    map(+1)
```

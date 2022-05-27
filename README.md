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

A module is an anonymous function.

```
# math
m = {}
m.pi = 3.14159

m.add = fn (x, y) { return x+y }

return m
```

```
math = require("math") # import math module
print = require("std").print

math.add(3, 9) # 12
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

## functional

```
fn filter(t, predicate) {
    res = {}
    for k, v in t {
        if predicate(v) {
            res[k] = v
        }
    }
    return res
}

fn map(t, f) {
    res = {}
    for k, v in t {
        res[k] = f(v)
    }
    return res
}

# `x.f(y)` is a syntax sugar for `f(x, y)`
{1, 2, 3, 4}.
    filter(fn (x) {return x>1}).
    map(fn (x) {return x+1}) # --> 3, 4, 5
```

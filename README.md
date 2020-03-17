# Hedgehog

Hedgehog is a simple dynamic programming language made with yacc and lex.

There is a post about the implementation of this interpreter in Chinese: [写一个简单解释器](https:#yangtau.me/programming-languages/simple-interpreter.html)

## Features so far

- Values

    ```
    a = 10              #int
    b = 3.14            #float
    c = true            #boolean
    d = null            #null
    s = 'Hello, World!' #string
    ```

- Controls

  There must be `{}` after `if`, `else if` and `else`, even if there is only one statement in the block.

    ```
    a = 10
    if a > 10 { # `()` is not necessary.
        b = a+20
    } else if a == 10 {
        b = a+10
    } else {
        b = a-10
    }
    puts(b)
    ```
- Loops

  There is no `while` loop, for `for` can replace it.

    ```
    for i=0; i<10; i=i+1 {
        puts(i)
        if i>=4 {break}
    }
    i = 0
    for i<10 {
        if i<5 {continue}
        puts(i)
    }
    ```
- Functions

    ```
    func Hello() {
        puts('Hello, world!')
    }
  
    Hello()
    ```
    ```
    func fbi(n) {
        a, b = 0, 1
        for a<n {
            puts(a)
            a, b = b, a+b
        }
    }
  
    puts('fib(100) =', fbi(100))
    ```
    ```
    func factorial(n) {
        if n==0 {return 1}
        return n*factorial(n-1)
    }
  
    puts(factorial(5))
    ```

- Operators
  
  Most operator in hedgehog is the same with c. However, there is only one `&` for `logical and`, and one `|` for `logical or`.
    ```
    b = 2
    a = 10
    if a>20 & b<10 {
        puts('`b` is less than 10 and `a` is greater than 20')
    }
    if a>20 | b<10 {
        puts('`b` is less than 10 or `a` is greater than 20')
    }
    ```


## Usage

Get start:
```
make
./bin/hedgehog example/values.hg
```

code examples:

```
puts('Hello world')
```

```
func fibonacci(n) {
    if n < 0 {
        return -1
    } else if n == 0 {
        return 0
    } else {
        a, b = 0, 1
        for j=1;j<n;j=j+1 {
            a, b = b, a+b
        }
        return b
    }
}

puts(fibonacci(20))

func fibonacci(n) {
    if n<0 {return -1}
    else if n==0 {return 0}
    else if n==1 {return 1}
    return fibonacci(n-1)+fibonacci(n-2)
}

puts(fibonacci(20))
```

## TODO

- [ ] Garbage collector
- [ ] Lists
- [ ] Maps

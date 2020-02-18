# Hedgehog

Hedgehog is a simple dynamic programming language made with yacc and lex.

There is a post about the implementation of this interpreter in Chinese: [自制简易解释器](https://yangtau.me/programming-languages/simple-interpreter.html)

## Features so far

- Value

    ```
    a = 10;              //int
    b = 3.14;            //float
    c = true;            //boolean
    d = null;            //null
    s = "Hello, World!"; //string
    ```

- Control 

  There must be `{}` after `if`, `elsif` and `else`, even if there is only one statement in the block.

    ```
    a = 10;
    if a > 10 { // `()` is not necessary.
        b = a+20;
    } elsif a==10 {
        b = a+10;
    } else {
        b = a-10;
    }
    print(b);
    ```
- Loops

  There is no `while` loop, for `for` can replace it.

    ```
    for i=0; i<10; i=i+1 {
        print(i);
        if i>=4 {break;}
    }
    i = 0;
    for i<10 {
        if i<5 {continue;}
        print(i);
    }
    ```
- Function

    ```
    func Hello() {
        print("Hello, world!");
    }
  
    Hello();
    ```
    ```
    func fbi(n) {
        a, b = 0, 1;
        for a<n {
            print(a);
            a, b = b, a+b;
        }
    }
  
    fbi(100);
    ```
    ```
    func factorial(n) {
        if n==0 {return 1;}
        return n*factorial(n-1);
    }
  
    print(factorial(5));
    ```

- Operator
  
  Most operator in hedgehog is the same with c. However, there is only one `&` for `logical and`, and one `|` for `logical or`.
    ```
    b = 2;
    a = 10;
    if a>20 & b<10 {
        print("`b` is less than 10 and `a` is greater than 20");
    }
    if a>20 | b<10 {
        print("`b` is less than 10 or `a` is greater than 20");
    }
    ```

## Compile:

```
yacc -d hedgehog.y
lex hedgehog.l
gcc -std=c99 -o hedgehog *.c
```

## Usage

run test:

`./hedgehog test/tes.hg`

code examples:


```
print("Hello world");
```
```
func fibonacci(n) {
    if n < 0 {
        return -1;
    } elsif n == 0 {
        return 0;
    } else {
        a, b = 0, 1;
        for j=1;j<n;j=j+1 {
            a, b = b, a+b;
        }
        return b;
    }
}

print(fibonacci(20));

func fibonacci(n) {
    if n<0 {return -1;}
    elsif n==0 {return 0;}
    elsif n==1 {return 1;}
    return fibonacci(n-1)+fibonacci(n-2);
}

print(fibonacci(20));
```

## TODO

- [ ] Garbage collector
- [ ] Lists
- [ ] Maps

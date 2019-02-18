# HEDGEHOG
Hedgehog is a simple dynamic programming language made with yacc and lex.

## Feature so far
- Value
    ```
    a = 10;
    b = 3.14;
    c = true;
    d = null;
    s="Hello, World!";
    ```
- Control
  There must be `{}` after `if`, `elsif` and `else`, even if there is only one.
    ```
    a = 10;
    if a > 10 {
        b = a+20;
    } elsif a==10 {
        b = a+10;
    } else {
        b = a-10;
    }
    print(b);// block has no local environment, 
             //so 'b' is a global variable.
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
  A function is also treated as a value.
    ```
    func Hello() {
        print("Hello, world!");
    }

    Hello();
    ```
    ```
    func fbi(n) {
        a, b = 0, 1
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
    And there is only one native function so far, `print`, which takes one value argument.
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

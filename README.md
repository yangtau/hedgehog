# HEDGEHOG
Hedgehog is a dynamic programming language made with yacc and lex.
It is too simple and may exist many bugs.
## Compile:
```
yacc -d hedgehog.y
lex hedgehog.l
gcc -o hedgehog *.c
```

## Usage
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
        a = 0;
        b = 1;
        func fn() {
            t = a;
            a = b;
            b = t+b;
        }
        for j=1;j<n;j=j+1 {fn();}
        return b;
    }
}

for i=0; i<10; i=i+1 {
    print(fibonacci(i));
}
```

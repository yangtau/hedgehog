# HEDGEHOG
Hedgehog is a dynamic programming language made with yacc and lex.
It is too simple and may exist many bugs.
## Compile:
```
yacc -d hedgehog.y
lex hedgehog.l
gcc -std=c99 -o hedgehog *.c
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
        for j=1;j<n;j=j+1 {
            t = a;
            a = b;
            b = t+b;
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

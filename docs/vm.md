# VM

Every file is an anonymous function that may return something.
The built-in function `require` will read a file, and call it as a function. 

The interpreter will compile every file into a chunk, which contains instructions, constants, and any necessary information.

## Chunks

```
+--------------+
|  constants   |
+--------------+
| instructions |
|  of the file |
|    closure   |
+--------------+
```

## Function Call

```
stack:
+---------------+---------+ <--- BASE
|  arg0, arg1   |         |
|  ..., argn    |         |
+---------------+         |
|  ret val0     |         |
|     ...       |         |
|  ret valn     |         |
+---------------+         |
|  local1       |         +--> R0-R255
|  local2       |         |
|  ...          |         |
|  localm       |         |
+---------------+---------+ <--- SP

call stack:
```

If we have the number of locals more than M, we use `push SP` to store a register, and then we restore it by `LOAD RX SP+N`.

### Call Instructions

```
call RX, N
```

call closure object stored in RX, which wants N arguments



## Instructions

```
ADD T RN RM
SUB T RN RM
MUL T RN RM
DIV T RN RM
MOD T RN RM
AND T RN RM
OR  T RN RM

GT  T RN RM  ; T = RN > RM
LT  T RN RM  ; T = RN < RM
GE  T RN RM  ; T = RN >= RM
LE  T RN RM  ; T = RN <= RM
EQ  T RN RM  ; T = RN == RM
NEQ  T RN RM ; T = RN != RM

; NEG T RN  ; P2
NOT T RN    ; T = not RN
```

```
if a {
   bar() 
} else {
   foo()
}

;--> a is stored in R1

jumpiffalse L1 R1
call bar
L1:
call foo

```

```
; label: 16 bits
jumpiffalse label RN; if !RN goto label
jump        label
```

```
for k,v in table {
    table[k] = v+1
}

; --> table is stored in R1

; constants:
; 1 get_iter(table) 
; 2 is_not_end(iter)
; 3 get_entry(iter)
; 4 next(iter)

; iter = get_iter(t)
; L1:
; if is_not_end(iter) {
;   k, v = get_entry(iter)
;   // dosomething with k, v
;   next(iter)
;   goto L1;
; }


; load help functions
LOAD R252 1        ; get_iter
LOAD R253 2        ; is_not_end
LOAD R254 3        ; get_entry
LOAD R255 4        ; next
call R252 1 1      ; R2 = get_iter(R1)
L1:
call R253 2 1      ; R3 = is_not_end(R2)
jumpfalse R3 L2    ; goto L2 if not R3 (is is_end(iter));
call R254 2 1      ; R3, R4 = get_entry(iter)
; do something with R3, R4
jump L1
L2:
``` 


```
## closure

```
a = 0
fn get() {
    return a
}

fn set(x) {
    a = x
}

return {.get=get, .set=set}
```

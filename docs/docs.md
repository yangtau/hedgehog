## TYPES
```hg
()     # nil, like `NULL` in C or `None` in Python
(1)    # 1, tuple will be unpacked
(1, 2) # tuple
```

## ASSIGN
```hg
a, b   = 1, 2     # ok
a, b   = (1, 2)   # error
(a, b) = (1, 2) # error
# I will consider to implement the last two later.
```

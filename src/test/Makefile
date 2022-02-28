YACC = bison
LEX  = flex
CC   = gcc
CFLAGS = -std=c11 -Wall -Wextra -rdynamic $(UDEF)

all: test

run: test
	./test_string
	./test_vm1
	./test_vm2
	./test_vm3

test: test_string test_vm1 test_vm2 test_vm3

test_string: string_test.c test.h ../string.h ../string.c
	$(CC) $(CFLAGS) ../string.c string_test.c -o test_string

test_vm1: vm_test.c test.h ../vm.c ../vm.h
	$(CC) $(CFLAGS) ../vm.c vm_test.c -o test_vm1

test_vm2: vm_test.c test.h ../vm.c ../vm.h
	$(CC) $(CFLAGS) -DHG_VM_JUMPTABLE ../vm.c vm_test.c -o test_vm2

test_vm3: vm_test.c test.h ../vm.c ../vm.h
	$(CC) $(CFLAGS) -DHG_VM_INLINE_THREADED ../vm.c vm_test.c -o test_vm3


clean:
	rm -f test_string
	rm -f test_vm1
	rm -f test_vm2
	rm -f test_vm3

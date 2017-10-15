all:
	gcc main.c -o pd2

test:
	./pd2 test.in test.out

gentest:
	dd if=/dev/urandom of=test.in count=2048
	dd if=/dev/urandom of=test-big.in count=204800

clean:
	rm -f pd2 test.out
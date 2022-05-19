.PHONY: clean
clean:
	rm -rf bin

bin/hash_djb2: hash_djb2.c
	mkdir bin
	gcc hash_djb2.c -o bin/hash_djb2

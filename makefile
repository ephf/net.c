a:
	gcc -I lib example/$(tar).c lib/*.c -o out
	./out
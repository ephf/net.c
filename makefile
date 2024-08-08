a:
	gcc -I lib example/$(tar).c lib/net.c -o out
	./out
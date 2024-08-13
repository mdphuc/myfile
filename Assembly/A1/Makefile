setup:
	nasm -f win64 hello.asm -o hello.o
	gcc hello.o -o hello.exe -lkernel32
	./hello.exe
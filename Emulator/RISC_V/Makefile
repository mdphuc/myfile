main:
	g++ cpu.cpp -o cpu -Wmain -Wformat-security -Wint-to-pointer-cast
	./cpu "./add-addi.bin"

compile:
	riscv64-unknown-elf-gcc -Wl,-Ttext=0x0 -nostdlib -march=rv64i -mabi=lp64 -o add-addi add-addi.s
	riscv64-unknown-elf-objcopy -O binary add-addi add-addi.bin
	rm -rf add-addi 

simple:
	riscv64-unknown-elf-gcc -S simple.c
	riscv64-unknown-elf-gcc -Wl,-Ttext=0x0 -nostdlib -march=rv64i -mabi=lp64 -o simple simple.s
	riscv64-unknown-elf-objcopy -O binary simple simple.bin
	rm -rf simple
	
# clean:
# 	rm -f add-addi
# 	rm -f add-addi.bin
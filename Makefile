ASM = nasm
CC = gcc
LD = ld
QEMU = qemu-system-i386

BUILD_DIR = build
OBJECTS = ${BUILD_DIR}/boot.o ${BUILD_DIR}/kernel.o ${BUILD_DIR}/serial.o

ASMFLAGS = -f elf32
CFLAGS = -m32 -c -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -I include
LDFLAGS = -m elf_i386 -T link.ld -nostdlib
QEMUFLAGS = -kernel ${BUILD_DIR}/kernel.bin -serial stdio

.PHONY: all clean run

all: mkbuild build run

mkbuild:
	@mkdir -p ${BUILD_DIR}

build: ${BUILD_DIR}/kernel.bin

${BUILD_DIR}/kernel.bin: $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

${BUILD_DIR}/boot.o: boot.asm | mkbuild
	$(ASM) $(ASMFLAGS) -o $@ $<

${BUILD_DIR}/serial.o: serial.c | mkbuild
	$(CC) $(CFLAGS) -o $@ $<

${BUILD_DIR}/kernel.o: kernel.c | mkbuild
	$(CC) $(CFLAGS) -o $@ $<

run: ${BUILD_DIR}/kernel.bin
	$(QEMU) $(QEMUFLAGS)

clean:
	rm -rf ${BUILD_DIR}
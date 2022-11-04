include Makefile.inc

MM=CUSTOM

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_DRIVERS=$(wildcard drivers/*.c)
SOURCES_IDT=$(wildcard idt/*.c)
SOURCES_C=$(wildcard c/*.c)

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c=.o)
OBJECTS_IDT=$(SOURCES_IDT:.c=.o)
OBJECTS_C=$(SOURCES_C:.c=.o)
LOADERSRC=loader.asm 

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_IDT) $(OBJECTS_C)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_DRIVERS) $(OBJECTS_IDT) $(OBJECTS_C)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_DRIVERS) $(OBJECTS_IDT) $(OBJECTS_C)

%.o: %.c
	$(GCC) $(GCCFLAGS) -D$(MM) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin drivers/*.o idt/*.o c/*.o

.PHONY: all clean

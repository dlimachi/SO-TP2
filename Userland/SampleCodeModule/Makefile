include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
#SOURCES=$(wildcard [^_]*.c)
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_C=$(wildcard c/*.c)
SOURCES_TESTS=$(wildcard tests/*.c)

OBJECTS=$(SOURCES:.c=.o) 
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o) 
OBJECTS_C=$(SOURCES_C:.c=.o) 
OBJECTS_TESTS=$(SOURCES_TESTS:.c=.o)
STATICLIBS=

all: $(MODULE)

$(MODULE): $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_C) $(OBJECTS_TESTS)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld -o ../$(MODULE) $^
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld --oformat=elf64-x86-64 -o ../0000-sampleCodeModule.elf $^

clean:
	rm -rf *.o *.bin asm/*.o c/*.o tests/*.o

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

.PHONY: all clean print

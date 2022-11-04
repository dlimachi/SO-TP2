include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard *.c) # ANTES DECIA [^_]*.c
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_C=$(wildcard c/*.c)
SOURCES_FUNC=$(wildcard functionalities/*.c)	
SOURCES_TESTS=$(wildcard tests/*.c)

OBJECTS=$(SOURCES:.c=.o) 
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o) 
OBJECTS_C=$(SOURCES_C:.c=.o)
OBJECTS_FUNC=$(SOURCES_FUNC:.c=.o)   
OBJECTS_TESTS=$(SOURCES_TESTS:.c=.o)

all: $(MODULE)

$(MODULE): $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_C)  $(OBJECTS_FUNC)  $(OBJECTS_TESTS)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld -o ../$(MODULE) $^
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld --oformat=elf64-x86-64 -o ../0000-sampleCodeModule.elf $^

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -I./c -I./functionalities -I./tests -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf *.o *.bin asm/*.o c/*.o tests/*.o

.PHONY: all clean print

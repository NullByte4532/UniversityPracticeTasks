ASM_BIN_FILE=asm.run
DEPS_ASM= Asm.o
EMU_BIN_FILE=emu.run
DEPS_EMU= Emu.o Stack.o
COMPILER=g++
all: $(ASM_BIN_FILE) $(EMU_BIN_FILE)
$(EMU_BIN_FILE): $(DEPS_EMU)
		$(COMPILER) $(DEPS_EMU) -o $(EMU_BIN_FILE)
$(ASM_BIN_FILE): $(DEPS_ASM)
		$(COMPILER) $(DEPS_ASM) -o $(ASM_BIN_FILE)
Stack.o: Stack.cxx
		$(COMPILER) -c Stack.cxx
Asm.o: Asm.cxx opcodes.hxx
		$(COMPILER) -c Asm.cxx
Emu.o: Emu.cxx
		$(COMPILER) -c Emu.cxx
clean:
		rm -f *.o *.run *.gch

ASM_BIN_FILE=asm.run
DEPS_ASM= Asm.o HashTable.o
EMU_BIN_FILE=emu.run
DEPS_EMU= Emu.o cpu.o Stack.o
COMPILER=g++
all: $(ASM_BIN_FILE) $(EMU_BIN_FILE) test.bin test2.bin
$(EMU_BIN_FILE): $(DEPS_EMU)
		$(COMPILER) $(DEPS_EMU) -o $(EMU_BIN_FILE)
$(ASM_BIN_FILE): $(DEPS_ASM)
		$(COMPILER) $(DEPS_ASM) -o $(ASM_BIN_FILE)
Stack.o: Stack.cxx
		$(COMPILER) -c Stack.cxx
HashTable.o: HashTable.cxx
		$(COMPILER) -c HashTable.cxx
Asm.o: Asm.cxx opcodes.hxx
		$(COMPILER) -c Asm.cxx
Emu.o: Emu.cxx
		$(COMPILER) -c Emu.cxx 
cpu.o: cpu.cxx
		$(COMPILER) -c cpu.cxx
test.bin: test.txt $(ASM_BIN_FILE)
		./$(ASM_BIN_FILE) test.txt test.bin
test2.bin: test2.txt $(ASM_BIN_FILE)
		./$(ASM_BIN_FILE) test2.txt test2.bin
clean:
		rm -f *.o *.run *.gch *.bin

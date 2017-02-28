#ifndef __opcodes_H_INCLUDED__ 
#define __opcodes_H_INCLUDED__
#define OP_MAX_LENGTH	6
#define ARG_MAX_LENGTH	9
#define OP_NOP		0x01
#define OP_PUSH		0x02
#define OP_POP		0x03
#define OP_ADD		0x04
#define OP_SUB		0x05
#define OP_MUL		0x06
#define OP_DIV		0x07
#define OP_LSHIFT	0x08
#define OP_RSHIFT	0x09
#define OP_JMP		0x0a
#define OP_JEZ		0x0b
#define OP_JNZ		0x0c
#define OP_JLZ		0x0d
#define OP_JGZ		0x0e
#define OP_CALL		0x0f
#define OP_RET		0x10
#define OP_HLT		0x00

#define R_EAX		0x01
#define R_EBX		0x02
#define R_ECX		0x03
#define R_EIP		0x04
#define R_IO		0x05

#define R_CODE		0x01
#define N_CODE		0x02
#define A_CODE		0x03
#define NI_CODE		0x04

#define ADDR_PREFIX 'h'
#endif

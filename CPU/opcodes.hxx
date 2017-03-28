#ifndef __opcodes_H_INCLUDED__ 
#define __opcodes_H_INCLUDED__
#define OP_MAX_LENGTH	6
#define ARG_MAX_LENGTH	9
#define CALL_STACK_SIZE 16
#define debug 0

#define OP_NOP		0x01
#define OP_PUSH		0x02
#define OP_POP		0x03
#define OP_ADD		0x04
#define OP_SUB		0x05
#define OP_MUL		0x06
#define OP_DIV		0x07
#define OP_JMP		0x08
#define OP_JEZ		0x09
#define OP_JNZ		0x0a
#define OP_JLZ		0x0b
#define OP_JGZ		0x0c
#define OP_CALL		0x0d
#define OP_RET		0x0e
#define OP_MOV		0x0f
#define OP_HLT		0x00

#define R_EAX		0x01
#define R_EBX		0x02
#define R_ECX		0x03
#define R_EIP		0x04
#define R_IO		0x05
#define R_ESP		0x06

#define R_CODE		0x01
#define N_CODE		0x02
#define A_CODE		0x03
#define NI_CODE		0x04
#define AI_CODE		0x05
#define RI_CODE		0x06
#define RA_CODE		0x07
#define RAI_CODE	0x08

#define ADDR_PREFIX 'h'
#define ADDR_I_PREFIX 'H'
#define LABEL_REF_PREFIX '&'
#define LABEL_DEF_PREFIX ':'

#define HEADER_H 0x45
#define HEADER_L 0x32
#define DEF_STACK_SIZE 256

#endif

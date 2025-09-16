#include<stdio.h>
#include "debug.h"
#include "value.h"

void disassemble_chunk(const Chunk* chunk, const char* name) {
    printf("== %s ==\n",name);
    for (int offset = 0 ; offset < chunk -> count;) {
        offset = disassemble_instruction(chunk,offset);
    }
}

static int simple_instruction(char * name, const int offset) {
    printf("%s\n",name);
    return offset + 1;
}

static int constant_instruction(const char* name, const Chunk* chunk, const int offset) {
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d '",name,constant);
    print_value(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}

int disassemble_instruction(const Chunk* chunk, const int offset) {
    printf("%04d ",offset);
    const int line = get_line(chunk,offset);
    const int prev_line = (offset > 0) ? get_line(chunk,offset - 1) : -1;
    if (offset > 0 && line == prev_line) {
        printf("  |  ");
    } else {
        printf("%4d ", line);
    }
    const uint8_t instruction = chunk -> code[offset];
    switch (instruction) {
        case OP_CONSTANT:
            return constant_instruction("OP_CONSTANT",chunk,offset);
        case OP_RETURN:
            return simple_instruction("OP_RETURN",offset);
        default:
            printf("Unknown opcode %d\n",instruction);
            return offset + 1;
    }
}


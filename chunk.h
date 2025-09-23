#ifndef clox_chunk_h
#define clox_chunk_h
#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_NOT,
    OP_RETURN,
    OP_NEGATE,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_PRINT,
    OP_POP,
    OP_DEFINE_GLOBAL,
    OP_GET_GLOBAL,
    OP_SET_GLOBAL,
    OP_GET_LOCAL,
    OP_SET_LOCAL,
} OpCode;

typedef struct {
    int line;
    int count;
} LineRun;

typedef struct {
    int count;
    int capacity;
    uint8_t* code;

    int line_count;
    int line_capacity;
    LineRun* lines;

    ValueArray constants;

} Chunk;

void init_chunk(Chunk* chunk);
void write_chunk(Chunk *chunk,uint8_t byte,int line);
void free_chunk(Chunk* chunk);
int add_constant(Chunk* chunk,Value value);
int get_line(const Chunk* chunk, int instruction_index);
void write_constant(Chunk* chunk,Value value,int line);

#endif

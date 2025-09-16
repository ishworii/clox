#ifndef clox_chunk_h
#define clox_chunk_h
#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_RETURN,
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

#endif

#include<stdlib.h>
#include "chunk.h"
#include "memory.h"


static void write_line_run(Chunk* chunk,int line) {
    if (chunk-> line_count > 0) {
        LineRun* last = &chunk->lines[chunk->line_count -1];
        if (last->line == line) {
            last->count++; //extend current run
        }
    }
    //need a new run
    if (chunk -> line_capacity < chunk->line_count + 1) {
        int old = chunk -> line_capacity;
        chunk -> line_capacity = GROW_CAPACITY(old);
        chunk -> lines = GROW_ARRAY(LineRun,chunk->lines,old,chunk->line_capacity);
    }
    chunk->lines[chunk->line_count].line = line;
    chunk->lines[chunk->line_count].count = 1;
    chunk->line_count += 1;

}

void init_chunk(Chunk* chunk) {
    chunk -> capacity = 0;
    chunk -> count = 0;
    chunk -> code = NULL;

    chunk -> line_capacity = 0;
    chunk -> line_count = 0;
    chunk -> lines = NULL;

    init_value_array(&chunk->constants);
}

void write_chunk(Chunk* chunk, const uint8_t byte, const int line) {
    if (chunk ->capacity < chunk-> count + 1) {
        const int old_capacity = chunk -> capacity;
        chunk -> capacity = GROW_CAPACITY(old_capacity);
        chunk -> code = GROW_ARRAY(uint8_t,chunk -> code,old_capacity,chunk->capacity);
    }
    chunk -> code[chunk -> count] = byte;
    chunk -> count++;
    write_line_run(chunk,line);
}

void free_chunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t,chunk->code,chunk->capacity);
    FREE_ARRAY(LineRun,chunk->lines,chunk->line_capacity);
    free_value_array(&chunk->constants);
    init_chunk(chunk);
}

int add_constant(Chunk* chunk, const Value value) {
    write_value_array(&chunk->constants,value);
    return chunk->constants.count - 1;
}

int get_line(const Chunk* chunk,int instruction_index) {
    int idx = instruction_index;
    for (int i = 0 ; i < chunk->line_count; i++) {
        if (idx < chunk->lines[i].count) {
            return chunk->lines[i].line;
        }
        idx -= chunk->lines[i].count;
    }
    //should not happen
    return -1;
}

void write_constant(Chunk* chunk, const Value value, const int line) {
    const int constant_index = add_constant(chunk,value);
    if (constant_index <= 0xFF) {
        write_chunk(chunk,OP_CONSTANT,line);
        write_chunk(chunk, (uint8_t)constant_index,line);
    }else {
        write_chunk(chunk,OP_CONSTANT_LONG,line);
        write_chunk(chunk,(uint8_t)(constant_index & 0xFF),line);
        write_chunk(chunk,(uint8_t)((constant_index >> 8) & 0xFF),line);
        write_chunk(chunk,(uint8_t)((constant_index >> 16) & 0xFF),line);

    }
}
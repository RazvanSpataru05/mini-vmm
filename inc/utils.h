#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

#include <sys/mman.h>

void die(const char* message);
void deallocate_memory(void* memory, size_t memory_size);

#endif
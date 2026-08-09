#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>

void die(const char* message);
void load_guest(const char* file_path, void* vm_memory, size_t buffer_size);
void deallocate_memory(void* memory, size_t memory_size);

#endif
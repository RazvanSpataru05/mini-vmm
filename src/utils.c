#include "utils.h"

void die(const char* message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

void deallocate_memory(void* memory, size_t memory_size)
{
    if (munmap(memory, memory_size) != 0) die("munmap error");
}
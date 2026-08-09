#include "utils.h"

void die(const char* message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

void load_guest(const char* file_path, void* vm_memory, size_t buffer_size)
{
    int guest_fd = open(file_path, O_RDWR);
    if (guest_fd < 0) die("open guest binary");

    char* p = (char*) vm_memory;
    for (;;)
    {
        int r = read(guest_fd, p, buffer_size);
        if (r <= 0) break;
        p += r;
    }
    close(guest_fd);
}

void deallocate_memory(void* memory, size_t memory_size)
{
    if (munmap(memory, memory_size) != 0) die("munmap error");
}


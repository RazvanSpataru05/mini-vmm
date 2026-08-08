#include <stdio.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/kvm.h>

#define RAM_SIZE 0x10000 //1MB of memory for VM
#define KVM_API_VERSION 12

int main()
{
    int kvm_fd = open("/dev/kvm", O_RDWR);
    int version;
    int vm_fd;
    if (kvm_fd < 0)
    {
        perror("open /dev/kvm");
        return 1;
    }

    version = ioctl(kvm_fd, KVM_GET_API_VERSION, 0);
    if (version != KVM_API_VERSION)
    {
        perror("KVM_GET_API_VERSION");
        return 1;
    }
    
    vm_fd = ioctl(kvm_fd, KVM_CREATE_VM, 0);
    if (vm_fd < 0)
    {
        perror("KVM_CREATE_VM");
        return 1;
    }
    void* virtual_machine_memory = mmap(NULL, RAM_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    if (virtual_machine_memory == MAP_FAILED)
    {
        perror("Error allocating memory for VM");
        return 1;
    }

    if (munmap(virtual_machine_memory, RAM_SIZE) != 0)
    {
        perror("Error deallocating mmeory for VM");
        return 1;
    }
    printf("VM memory deallocated successfully\n");
    return 0;
}
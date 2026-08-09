#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/kvm.h>

#define RAM_SIZE 0x10000 //1MB of memory for VM
#define PAGE_SIZE 4096 // 4096KB/ memory page
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
    
    void* virtual_machine_memory = mmap(NULL, RAM_SIZE, PROT_READ | PROT_WRITE,
         MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);

    if (virtual_machine_memory == MAP_FAILED)
    {
        perror("Error allocating memory for VM");
        return 1;
    }

    struct kvm_userspace_memory_region mem = {
        .slot = 0,
        .guest_phys_addr = 0,
        .memory_size = RAM_SIZE,
        .userspace_addr = (uintptr_t)virtual_machine_memory
    };
    ioctl(vm_fd, KVM_SET_USER_MEMORY_REGION, &virtual_machine_memory);

    int vcpu_fd = ioctl(vm_fd, KVM_CREATE_VCPU, 0);
    if (vcpu_fd < 0)
    {
        perror("KVM_CREATE_VCPU");
        return 1;
    }

    int guest_fd = open("guest/guest", O_RDWR);
    if (guest_fd < 0)
    {
        perror("guest FD");
        return 1;
    }
    char* p = (char*) virtual_machine_memory;
    for(;;)
    {
        int r = read(guest_fd, p, PAGE_SIZE);
        if (r <= 0) break;

        p += r;
    }
    close(guest_fd);

    if (munmap(virtual_machine_memory, RAM_SIZE) != 0)
    {
        perror("Error deallocating mmeory for VM");
        return 1;
    }

    printf("VM memory deallocated successfully\n");
    return 0;
}
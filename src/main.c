#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/kvm.h>

#include "utils.h"
#include "kvm.h"

#define RAM_SIZE 0x10000 //1MB of memory for VM
#define PAGE_SIZE 4096 // 4096KB / memory page
#define KVM_API_VERSION 12

int main()
{
    int kvm_fd = kvm_init(KVM_API_VERSION);
    int vm_fd = kvm_create_vm(kvm_fd);
    void* virtual_memory = kvm_setup_memory(vm_fd, RAM_SIZE);
    int vcpu_fd = kvm_create_vcpu(vm_fd);

    deallocate_memory(virtual_memory, RAM_SIZE);
    return 0;
}
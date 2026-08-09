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
#define BUFFER_SIZE 4096 // 4096KB / memory page
#define KVM_API_VERSION 12
#define GUEST_PATH "guest/guest"

int main()
{
    int kvm_fd = kvm_init(KVM_API_VERSION);
    int vm_fd = kvm_create_vm(kvm_fd);
    void* virtual_memory = kvm_setup_memory(vm_fd, RAM_SIZE);
    int vcpu_fd = kvm_create_vcpu(vm_fd);
    load_guest(GUEST_PATH, virtual_memory, BUFFER_SIZE); // loads guest program into VM

    struct kvm_run* run = kvm_setup_vcpu_run(kvm_fd, vcpu_fd);
    ioctl(vcpu_fd, KVM_RUN, 0);

    deallocate_memory(virtual_memory, RAM_SIZE);
    return 0;
}
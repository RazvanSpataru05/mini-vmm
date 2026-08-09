#include "kvm.h"

int kvm_init(int expected_version)
{
    int kvm_fd = open("/dev/kvm", O_RDWR);
    if (kvm_fd < 0) die("Open /dev/kvm error");

    int version = ioctl(kvm_fd, KVM_GET_API_VERSION, 0);
    if (version != expected_version) die("KVM API version different from expected");

    return kvm_fd;
}

int kvm_create_vm(int kvm_fd)
{
    int vm_fd = ioctl(kvm_fd, KVM_CREATE_VM, 0);
    if (vm_fd < 0) die("KVM_CREATE_VM error");

    return vm_fd;
}

void* kvm_setup_memory(int vm_fd, size_t ram_size)
{
    void* vm_memory = mmap(NULL, ram_size, PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
    if (vm_memory == MAP_FAILED) die("Error allocating memory for VM");

    struct kvm_userspace_memory_region memory = {
        .slot = 0,
        .guest_phys_addr = 0,
        .memory_size = ram_size,
        .userspace_addr = (uintptr_t) vm_memory
    };

    ioctl(vm_fd, KVM_SET_USER_MEMORY_REGION, &memory);
    return vm_memory;
}

int kvm_create_vcpu(int vm_fd)
{
    int vcpu_fd = ioctl(vm_fd, KVM_CREATE_VCPU, 0);
    if (vcpu_fd < 0) die("KVM_CREATE_VCPU error");

    return vcpu_fd;
}

void* kvm_setup_vcpu_run(int kvm_fd, int vcpu_fd)
{
    int mmap_size = ioctl(kvm_fd, KVM_GET_VCPU_MMAP_SIZE, 0);
    if (mmap_size < 0) die("KVM_GET_VCPU_MMAP_SIZE error");

    struct kvm_run* run = mmap(NULL, mmap_size, PROT_READ | PROT_WRITE,
    MAP_ANONYMOUS | MAP_PRIVATE, vcpu_fd, 0);
    if (run == MAP_FAILED) die("mmap error");

    return run;
}
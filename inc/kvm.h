#ifndef KVM_H
#define KVM_H

#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/kvm.h>

#include "utils.h"

int kvm_init(int expected_version);
int kvm_create_vm(int kvm_fd);
void* kvm_setup_memory(int vm_fd, size_t ram_size);
int kvm_create_vcpu(int vm_fd);
void* kvm_setup_vcpu_run(int kvm_fd, int vcpu_fd);

#endif
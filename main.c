#include <stdio.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/kvm.h>

#define RAM_SIZE 0x10000

int main()
{
    int kvm_fd = open("/dev/kvm", O_RDWR);
    if (kvm_fd < 0)
    {
        perror("open /dev/kvm");
        return 1;
    }
    int version = ioctl(kvm_fd, KVM_GET_API_VERSION, 0);
    printf("KVM Version: %d\n", version);
    
    int vm_fd = ioctl(kvm_fd, KVM_CREATE_VM, 0);
    return 0;
}
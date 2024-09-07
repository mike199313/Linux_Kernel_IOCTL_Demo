#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define DEVICE_NAME "/dev/ioctl_example"
#define IOCTL_MAGIC 'k'
#define IOCTL_SET_VALUE _IOW(IOCTL_MAGIC, 0, int)
#define IOCTL_GET_VALUE _IOR(IOCTL_MAGIC, 1, int)

int main() {
    int fd;
    int value;

    fd = open(DEVICE_NAME, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return EXIT_FAILURE;
    }

    value = 42;
    if (ioctl(fd, IOCTL_SET_VALUE, &value) < 0) {
        perror("Failed to set value");
        close(fd);
        return EXIT_FAILURE;
    }
    printf("Set device value to %d\n", value);

    if (ioctl(fd, IOCTL_GET_VALUE, &value) < 0) {
        perror("Failed to get value");
        close(fd);
        return EXIT_FAILURE;
    }
    printf("Got device value: %d\n", value);

    close(fd);
    return EXIT_SUCCESS;
}

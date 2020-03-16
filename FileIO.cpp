#include "mbed.h"
#include <stdio.h>
#include <errno.h>

#include "BlockDevice.h"

// Maximum number of elements in buffer
#define BUFFER_MAX_LEN 10

// This will take the system's default block device
BlockDevice *bd = BlockDevice::get_default_instance();

// Instead of the default block device, you can define your own block device.
// For example: HeapBlockDevice with size of 2048 bytes, read size 1, write size 1 and erase size 512.
// #include "HeapBlockDevice.h"
// BlockDevice *bd = new HeapBlockDevice(2048, 1, 1, 512);


// This example uses LittleFileSystem as the default file system
#include "LittleFileSystem.h"
LittleFileSystem fs("fs");

// Uncomment the following two lines and comment the previous two to use FAT file system.
// #include "FATFileSystem.h"
// FATFileSystem fs("fs");



// Entry point for the example
int main() {
    printf("--- Mbed OS filesystem example ---\n");

    // Try to mount the filesystem
    printf("Mounting the filesystem... ");
    fflush(stdout);
    int err = fs.mount(bd);
    printf("%s\n", (err ? "Fail :(" : "OK"));
    if (err) {
        // Reformat if we can't mount the filesystem
        // this should only happen on the first boot
        printf("No filesystem found, formatting... ");
        fflush(stdout);
        err = fs.reformat(bd);
        printf("%s\n", (err ? "Fail :(" : "OK"));
        if (err) {
            error("error: %s (%d)\n", strerror(-err), err);
        }
    }


    File f;
    f.open(&fs, "/fs/test.txt", O_RDWR);
    f.write("TEST", 3);
    f.close();

    char buffer[10] = {0,};
    f.open(&fs, "/fs/test.txt", O_RDWR);
    f.read(buffer, 10);
    printf("Read DATA: %s", buffer);

    // Tidy up
    printf("Unmounting... ");
    fflush(stdout);
    err = fs.unmount();
    printf("%s\n", (err < 0 ? "Fail :(" : "OK"));
    if (err < 0) {
        error("error: %s (%d)\n", strerror(-err), err);
    }

    printf("Mbed OS filesystem example done!\n");
}


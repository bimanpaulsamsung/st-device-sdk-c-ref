#include "mbed.h"
#include <stdio.h>
#include <errno.h>

#include "BlockDevice.h"

// Maximum number of elements in buffer
#define BUFFER_MAX_LEN 10

// This will take the system's default block device
static BlockDevice *bd = BlockDevice::get_default_instance();

// Instead of the default block device, you can define your own block device.
// For example: HeapBlockDevice with size of 2048 bytes, read size 1, write size 1 and erase size 512.
// #include "HeapBlockDevice.h"
// BlockDevice *bd = new HeapBlockDevice(2048, 1, 1, 512);


// This example uses LittleFileSystem as the default file system
#include "LittleFileSystem.h"
static LittleFileSystem fs("fs");

// Uncomment the following two lines and comment the previous two to use FAT file system.
// #include "FATFileSystem.h"
// FATFileSystem fs("fs");


// Set up the button to trigger an erase
InterruptIn irq(BUTTON1);
void erase() {
    printf("Initializing the block device... ");
    fflush(stdout);
    int err = bd->init();
    printf("%s\n", (err ? "Fail :(" : "OK"));
    if (err) {
        error("error: %s (%d)\n", strerror(-err), err);
    }

    printf("Erasing the block device... ");
    fflush(stdout);
    err = bd->erase(0, bd->size());
    printf("%s\n", (err ? "Fail :(" : "OK"));
    if (err) {
        error("error: %s (%d)\n", strerror(-err), err);
    }

    printf("Deinitializing the block device... ");
    fflush(stdout);
    err = bd->deinit();
    printf("%s\n", (err ? "Fail :(" : "OK"));
    if (err) {
        error("error: %s (%d)\n", strerror(-err), err);
    }
}


// Entry point for the example
int main() {
    printf("--- Mbed OS filesystem example ---\n");

    // Setup the erase event on button press, use the event queue
    // to avoid running in interrupt context
//    irq.fall(mbed_event_queue()->event(erase));

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

    // Open the numbers file
    printf("Opening \"/fs/numbers.txt\"... ");
//    fflush(stdout);
    printf("Opening \"/fs/numbers.txt\"... ");


    int fd = open("/fs/numbers.txt", O_RDWR, 0644);
    printf("%s\n", (fd<=0 ? "Fail :(" : "OK"));

    if (!fd) {
        // Create the numbers file if it doesn't exist
        printf("No file found, creating a new file... ");
        fflush(stdout);
        fd = open("/fs/numbers.txt", O_RDWR | O_CREAT, 0644);
        printf("%s\n", (!fd ? "Fail :(" : "OK"));
        if (fd<=0) {
            error("error: %s (%d)\n", strerror(errno), -errno);
        }

        for (int i = 0; i < 10; i++) {
            printf("\rWriting numbers (%d/%d)... ", i, 10);
            fflush(stdout);
            char buf[100] = {0, };
            int length = snprintf(buf, 100, "    %d\n", i);

            err = write(fd, buf, length);
            if (err < 0) {
                printf("Fail :(\n");
                error("error: %s (%d)\n", strerror(errno), -errno);
            }
        }
        printf("\rWriting numbers (%d/%d)... OK\n", 10, 10);
    }


    // Close the file which also flushes any cached writes
    printf("Closing \"/fs/numbers.txt\"... ");
    fflush(stdout);
    err = close(fd);
    printf("%s\n", (err < 0 ? "Fail :(" : "OK"));
    if (err < 0) {
        error("error: %s (%d)\n", strerror(errno), -errno);
    }

    // Display the root directory
    printf("Opening the root directory... ");
    fflush(stdout);
    DIR *d = opendir("/fs/");
    printf("%s\n", (!d ? "Fail :(" : "OK"));
    if (!d) {
        error("error: %s (%d)\n", strerror(errno), -errno);
    }

    printf("root directory:\n");
    while (true) {
        struct dirent *e = readdir(d);
        if (!e) {
            break;
        }

        printf("    %s\n", e->d_name);
    }

    printf("Closing the root directory... ");
    fflush(stdout);
    err = closedir(d);
    printf("%s\n", (err < 0 ? "Fail :(" : "OK"));
    if (err < 0) {
        error("error: %s (%d)\n", strerror(errno), -errno);
    }

    // Display the numbers file
    printf("Opening \"/fs/numbers.txt\"... ");
    fflush(stdout);

    fd = open("/fs/numbers.txt", O_RDONLY);
    printf("%s\n", (fd<=0 ? "Fail :(" : "OK"));
    if (fd<=0) {
        error("error: %s (%d)\n", strerror(errno), -errno);
    }

    int length = 500;
	char* data = (char*)malloc(length + 1);
	ssize_t size = read(fd, data, length);
    printf("numbers:\n");

    printf("%s\n", data);
    free(data);
    printf("\rClosing \"/fs/numbers.txt\"... ");
    fflush(stdout);
    err = close(fd);
    printf("%s\n", (err < 0 ? "Fail :(" : "OK"));
    if (err < 0) {
        error("error: %s (%d)\n", strerror(errno), -errno);
    }

    //Remove File
    printf("Deleting File... ");
	int ret = remove("/fs/numbers.txt");
	if (ret != 0) {
		printf("ERROR: Delete Failed  [0x%x]\n", ret);
	}

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


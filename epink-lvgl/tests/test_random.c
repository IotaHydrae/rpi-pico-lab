#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define EPINK_WIDTH 200
#define EPINK_HEIGHT 200

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static uint8_t epink_disp_buffer[EPINK_WIDTH * EPINK_HEIGHT / 8] = {0};

int main(int argc, char **argv)
{
    int fd;

    fd = open("/dev/urandom", O_RDONLY);

    read(fd, epink_disp_buffer, ARRAY_SIZE(epink_disp_buffer));

    for(int i=0;i<ARRAY_SIZE(epink_disp_buffer);i++){
        printf("%x ", epink_disp_buffer[i]);
    }
    printf("\n");

    return 0;
}
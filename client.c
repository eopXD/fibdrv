#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdint.h>
#include "uint128_XD.h"

#define FIB_DEV "/dev/fibonacci"

#define NINETEEN 10000000000000000000ull
typedef unsigned long long ull;
typedef unsigned __int128 uint128_t;
void print128_t ( uint128_t q ) {
    ull hi, lo;
    hi = q / NINETEEN;
    lo = q % NINETEEN;
    if ( hi == 0ull ) {
        printf("%llu\n", lo);
    }
    else {
        printf("%llu%019llu\n", hi, lo);
    }
}
uint128_t XD_to_std ( uint128_XD y ) {
    uint128_t x = y.hi;
    x = (x << 64) + y.lo;
    return x;
}
void print128_XD ( uint128_XD q ) {
    print128_t(XD_to_std(q));
}

int main()
{
    long long sz;
    unsigned long long buf[2];
    char write_buf[] = "testing writing";
    int offset = 100; /* TODO: try test something bigger than the limit */

    int fd = open(FIB_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }

    for (int i = 0; i <= offset; i++) {
        sz = write(fd, write_buf, strlen(write_buf));
        printf("Writing to " FIB_DEV ", returned the sequence %lld\n", sz);
    }

    for (int i = 0; i <= offset; i++) {
        lseek(fd, i, SEEK_SET);
        sz = read(fd, buf, 1);
        printf("Reading from " FIB_DEV
               " at offset %d, returned the sequence "
               "0x%llx%016llx.\n"
               i, buf[1], buf[0]);
    }

    for (int i = offset; i >= 0; i--) {
        lseek(fd, i, SEEK_SET);
        sz = read(fd, buf, 1);
        printf("Reading from " FIB_DEV
               " at offset %d, returned the sequence "
               "0x%llx%016llx.\n"
               i, buf[1], buf[0]);
    }

    close(fd);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <limits.h>    /* for PAGESIZE */

#define PAGESIZE 4096


int main(void)
{
    int N=5;
    int *ptr = mmap(NULL, 1024,
     PROT_READ | PROT_WRITE | PROT_EXEC , MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 );

    if(ptr == MAP_FAILED){
        printf("Mapping Failed\n");
        return 1;
    }
    printf("pepepepe %ld, %ld, %ld, %ld", EINVAL, EFAULT, ENOMEM, EACCES);
    if (mprotect(ptr, 0xFFFFFF, PROT_READ) != 0) {
        perror("Couldn’t mprotect");
        exit(errno);
    }
    printf("Address 0x%lx became read only", ptr);
    exit(0);
}

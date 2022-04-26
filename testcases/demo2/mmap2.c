#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
int main(){

int N=0x10000000000;
int *ptr = mmap ( NULL, N*1024,
 PROT_READ | PROT_WRITE | PROT_EXEC , MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 );


printf("My pid:%d\n", getpid());

strncpy(ptr, "Test", 4);
printf("\n");

sleep(1000);
return 0;
}

#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
int main(){

int N=5;
int *ptr = mmap ( NULL, N*sizeof(int),
 PROT_READ | PROT_WRITE | PROT_EXEC , MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 );

if(ptr == MAP_FAILED){
    printf("Mapping Failed\n");
    return 1;
}
printf("My pid:%d\n", getpid());
printf("Flag value is %d\n", MAP_PRIVATE| MAP_ANONYMOUS);

printf("memory address recieved is %lx", ptr);
printf("\n");
int err = munmap(ptr, 10*sizeof(int));
if(err != 0){
    printf("UnMapping Failed\n");
    return 1;
}
sleep(1000);
return 0;
}

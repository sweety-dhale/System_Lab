#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/syscall.h>

int main(void){
	
 //TAKE INPUT FROM USER
    char *name;
	scanf("%[^\n]",name);
   
 //SYSTEM CALL TO MYSYSCALL AT LINE NO 333 IN HEADER FILE
    long sta = syscall(333, name, 50);

	// PRINT THE LAST LINE OF OUTPUT
	system("dmesg | tail -1");
     return 0;
}

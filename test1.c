#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "zffqueue.h"


int main(){
	zffqueue_init(8, 100);
	unsigned long i=0;
	while(1){
		printf("putting %d\n", i);
		zffqueue_put(&i, 8);	
		i++;
		sleep(2);
	}

	return 0;	
}

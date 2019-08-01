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
	zffqueue_map();
	unsigned long i;
	while(1){
		zffqueue_get(&i, 8);	
		printf("getting %d\n", i);
		sleep(1);
	}

	return 0;	
}


#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <zffqueue.h>


int main(){
	zffqueue_map();
	unsigned long *p;
	while(1){
		p = (unsigned long*)zffqueue_get();	
		if(p != NULL){
			printf("getting %d\n", *p);
			zffqueue_pop();
		}
		sleep(1);
	}

	return 0;	
}


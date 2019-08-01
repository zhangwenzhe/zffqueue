#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

struct zffqueue{
	int rp;
	int wp;
	int n;
	unsigned long one_sz;
	char buf[0];
};

static struct zffqueue * zp = NULL;

int zffqueue_init(unsigned long sz, int n){
	int ret;
	int fd = shm_open("zffqueue", O_RDWR|O_CREAT, 0777);
	if(fd < 0){
		printf("error opening file\n");
		exit(1);
	}
	ret = ftruncate(fd, sz*n + sizeof(struct zffqueue));
	if(ret < 0){
		printf("error ftruncate\n");
		exit(1);
	}

	zp = (struct zffqueue *)mmap(NULL, sz*n +sizeof(struct zffqueue), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if((void *)zp == -1){
		printf("error mmap\n");
		exit(1);
	}

	zp->one_sz = sz;
	zp->n = n;
	zp->rp = zp->wp = 0;

	return 0;	
}


int zffqueue_map(){
	int ret;
	unsigned long sz;
	int fd = shm_open("zffqueue", O_RDWR|O_CREAT, 0777);
	if(fd < 0){
		printf("error opening file\n");
		exit(1);
	}

	zp = (struct zffqueue *)mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if((void *)zp == -1){
		printf("error mmap\n");
		exit(1);
	}

	sz = zp->one_sz * zp->n;
	if(sz + sizeof(struct zffqueue) <= 4096){
		return 0;
	}

	munmap(zp, 4096);
	zp = (struct zffqueue *)mmap(NULL, sz + sizeof(struct zffqueue), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if((void *)zp == -1){
		printf("error mmap\n");
		exit(1);
	}

	return 0;	
}

int zffqueue_put(void *p, unsigned long sz){
	void * dst;
	printf("before put wp %d\n", zp->wp);
	if(sz != zp->one_sz){
		printf("error put object sz\n");
		return -1;
	}
	if(((zp->wp + 1)%zp->n) == zp->rp){
		printf("queue full\n");
		return -1;
	}
	dst = zp->buf + zp->wp*zp->one_sz;
	memcpy(dst, p, sz);
	zp->wp = (zp->wp + 1)%zp->n;

	printf("after put wp %d\n", zp->wp);
	return 0;
}

int zffqueue_get(void *p, unsigned long sz){
	void * src;
	printf("before get rp %d\n", zp->rp);
	if(sz != zp->one_sz){
		printf("error get object sz\n");
		return -1;
	}
	if(zp->wp == zp->rp){
		printf("queue empty\n");
		return -1;
	}
	src = zp->buf + zp->rp*zp->one_sz;
	memcpy(p, src, sz);
	zp->rp = (zp->rp + 1)%zp->n;
	printf("after get rp %d\n", zp->rp);
	return 0;
}


/*
int main(){
	zffqueue_init(8, 10);
	int i;
	unsigned long buf[10];	
	unsigned long tmp;
	for(i=0; i<10; i++){
		buf[i] = i;
		zffqueue_put(&buf[i], 8);
	}
	zffqueue_map();
	for(i = 0; i<10; i++){
		zffqueue_get(&tmp, 8);
		printf("get %d\n", tmp);
	}

	return 0;
}
*/

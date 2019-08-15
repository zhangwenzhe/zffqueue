
int zffqueue_init(unsigned long sz, int n, int linesz);
int zffqueue_map();
int zffqueue_put(void *p);
void * zffqueue_get();
int zffqueue_pop();
unsigned long zffqueue_get_onesz();
int zffqueue_get_linesz();

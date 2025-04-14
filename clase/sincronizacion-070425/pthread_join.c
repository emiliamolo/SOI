#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* hello_world(void* v) {
	printf("Hello, World! %d\n", *(int*)v);
	
	sleep(2); // computando algo...

	int *ret = malloc(sizeof(int));
	*ret = 30;
	return ret; //pthread_exit(ret);
}

int main() {
	pthread_t id;
	int v = 4;
	
	puts("Creating thread");
	
	pthread_create(&id, NULL, hello_world, &v);
	
	void *retval;
    pthread_join(id, &retval);
    
    printf("Thread returned %d\n", *(int*)retval, retstatus);
    free(retval);
	
	puts("Main running...");
	
	return 0;
}
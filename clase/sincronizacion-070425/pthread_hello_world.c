#include <stdio.h>
#include <pthread.h>

void* hello_world(void* v) {
	printf("Hello, World! %d\n", *(int*)v);
	// poner un fflush acá tampoco sirve porque probablemente el padre muere antes que se ejecute, y falla
	
	//pthread_exit(0);
	return NULL;
}

int main() {
	pthread_t id;
	int v = 4;
	
	puts("Creating thread");
	
	pthread_create(&id, NULL, hello_world, &v);
	
	sleep(1); // si o si necesito un sleep -> muere primero el padre, antes de ejecutar el pthread sino
	
	puts("Main running...");
	
	return 0;
}
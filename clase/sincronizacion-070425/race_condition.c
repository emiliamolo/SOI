#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void characterAtATime(char *str) {
	setbuf(stdout, NULL);
	char *ptr;
	int c;
	for(ptr = str; (c = *ptr++); ) {
		putc(c, stdout);
	}
}

int main() {
	pid_t pid = fork();
	if (!pid) {
		characterAtATime("output from child\n");
	}
	else {
		characterAtATime("output from parent\n");
	}
	return 0;
}
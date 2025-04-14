#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void seek_write(int fd, off_t x, char v) {
	lseek(fd, x ,SEEK_SET);
	write(fd,&v, 1);
}

char seek_read(int fd, off_t x) {
	lseek(fd, x, SEEK_SET);
	char buf;
	read(fd, &buf, 1);
	return buf;
}

int main() {
	int fd = open("/storage/emulated/0/Documents/hola.txt", O_RDWR);
	
	pid_t pid = fork();
		
	if (!pid) {
	    char c = seek_read(fd, 13);
		printf("child leyó: %c\n", c);
		seek_write(fd, 13, 'a'); // Puede haber competencia con los lseek, de forma que primero se ejecute correctamente un seek_write entero, luego un lseek de un write se ejecute, se ejecute read que mueve el cabezal, y luego termina el ultimo write, escribiendo en la próxima posición, y se pueden escribir ambas letras en posiciones consecutivas
		// ESO ES UN EJEMPLO, PUEDE SUCEDER SI TANTO UN READ COMO UN WRITE LO MUEVE
		// Estas cosas pasan porque seek_write ni seek_read NO es atómico!
	}
	else {
	    char c = seek_read(fd,13);
		printf("parent leyó: %c\n", c);
		seek_write(fd, 13, 'b');
	}
	close(fd);
	
	/*
	printf("3 %c !!!!\n", seek_read(fd, 3));
	printf("10 %c !!!!\n", seek_read(fd, 10));	
	seek_write(fd, 10, 'h');
	printf("5 %c !!\n", seek_read(fd, 5));
	printf("10 %c !!!!\n", seek_read(fd, 10));
	
	for (int i = 0; i < 20; i++)
	    printf("%c", seek_read(fd, i));
	*/
	    
	return 0;
}
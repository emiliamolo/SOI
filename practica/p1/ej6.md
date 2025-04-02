

6)
Es getchar() una llamada al sistema? Por que si o por que no? Como funciona ungetc()?

No, getchar() no es una llamada al sistema. Lo que hace es usar  la biblioteca estándar de C, que maneja archivos y entradas de forma más eficiente. En lugar de pedirle al sistema operativo un solo carácter cada vez (lo cual sería lento), lo que realmente hace es leer varios caracteres de golpe usando un búfer y luego ir entregándolos de a uno cuando se los piden.

> https://stackoverflow.com/questions/9544151/how-getchar-is-implemented 

- By definition, getchar() uses the stream infrastructure of the standard C library i.e. FILE and its friendly functions. In most modern C library implementations, the file streams are buffered to a degree, with the buffer size and behavior usually being tunable via setvbuf().
- The function read(2) is usually implemented as a system call to the operating system kernel. Although the internal (and funky) details of such a thing shall not be discused here, the overall idea is that system calls are (usually) not something cheap.
> https://stackoverflow.com/questions/32131873/equivalent-of-fgetc-with-unix-file-descriptors
- It would be inefficient for both the userspace application and the kernel to do a system call just to get a single character from a file descriptor.
- For instance, fgetc(3) usually ends up doing some buffering inside the structure of the FILE object. This means that the internal read(2) from fgetc(3) wouldn't just read a single character, but rather it'll try to get more for the sake of efficiency.

ungetc() "devuelve" un caracter dado a un stream de entrada especificado, es decir, inserta un caracter en la "cabeza" del mismo. Uno de los propósitos de esta función es poder justamente "deshacer" una operación de getchar(). Esto puede ser útil si se quiere procesar una entrada, como leer un número, y que se reciba un caracter ya no deseado en el punto en cuestión del código (que se reciba un caracter que no sea dígito, en el ejemplo dado). El caracter insertado luego puede ser leído del stream por otra función. Está garantizado un buffer en el stream de al menos tamaño 1, para poder insertar el caracter en el stream, aunque el espacio disponible varía según el sistema y el entorno.

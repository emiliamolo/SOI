/* Lea un número N por teclado y luego imprima de forma aleatoria los números desde el 1 a N en grupos de 3.
   El programa debe usar al menos dos procesos y algún mecanismo de comunicación entre ellos para lograr la tarea.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

void swap(int *a, int *b) {
    int c = *b;
    *b = *a;
    *a = c;
}

int* mezclar(int* nums, int n){
    srandom(time(NULL));

    int it = n*n;
    for(int i=0; i<it; i++){
        int a = random() % n;
        int b = random() % n;
        swap(nums+a, nums+b);
    }

    return nums;
}

int main(int argc, char *argv[]){
    if (argc != 2) {
        printf("Uso: ./randomizer [cantidad de números]\n");
        return 0;
    }

    int n = atoi(argv[1]);
    
    int fd[2];
    pipe(fd);

    int pid = fork();
    if(pid == 0){
        close(fd[0]); // cierro lectura
        int* nums = malloc(sizeof(int) * n);
        for(int i=0; i<n; i++) nums[i] = i+1;
        nums = mezclar(nums, n);
        
        for(int i=0;i<n;i++)
            write(fd[1], nums+i, sizeof(int));

        close(fd[1]);
        free(nums);
    } else{
        close(fd[1]); // cierro escritura
        for(int i=0; i<n;i++){
            int num;
            read(fd[0], &num, sizeof(int));
            printf("%d ", num);
            if(i%3 == 2){
                printf("\n");
            }
        }
        if (n % 3) {
            printf("\n");
        }
        
        int status;
        wait(&status);
        close(fd[0]);
    }
    return 0;
}
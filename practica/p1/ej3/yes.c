#include <stdio.h>
#include <unistd.h>
int main(){
    while(1){
        write(1, "yes\n", 4);
    }
    
    return 0;
}
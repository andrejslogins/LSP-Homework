#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *test;
    int i = 0;
    printf("allocating...\n");
    for(i = 0;;i++){
        i+=10000;
        test = malloc(i*1024);
        printf("%s%d\n", test,i);
        free(test);

    }

// ...

    printf("done\n");
    return 0;
}

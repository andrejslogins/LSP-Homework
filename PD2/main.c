#include <stdio.h>
#include <stdlib.h>

// Andrejs Logins
// al13005

int main(int argc, char *argv[]) {
    if(argv[1] != NULL && argv[2]!= NULL){
        copyfile(argv[1],argv[2]);
    } else {
        printf("no args");
    }
    return 0;
}

long int get_file_size(char *filename){
    FILE *in = fopen(filename, "r");
    if(in){
        long int file_size;
        fseek(in,0,SEEK_END);
        file_size = ftell(in);
        fclose(in);
        return file_size;
    } else {
        return -1;
    }
}

void copyfile(char *input, char *output){
    long int in_size = get_file_size(input);
    if (in_size != -1){
        char *buffer = NULL;
        FILE *in, *out;
        in = fopen(input,"r");
        if(in) {
            buffer = (char*) malloc(sizeof(char) * (in_size+1));
            buffer[in_size] = '\0';
            out = fopen(output, "w");
            if(out){
                fwrite(buffer,sizeof(char),in_size,out);
                free(buffer);
                fclose(out);
            }
            fclose(in);

        }
    }

}


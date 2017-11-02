#include <stdio.h>
#include <dirent.h>
#include <memory.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <openssl/md5.h>

typedef struct file {
    char *filePath;
    char *fileName;
    unsigned *fileSize;
    struct timespec lastEdited;
    char *fileMD5Hash;

    struct file *next;
    struct file *prev;
}file;

file *first = NULL;
file *last = NULL;

void setFileValues(file *file){
    file->next = NULL;
    file->prev = NULL;
    file->fileMD5Hash = NULL;
    file->fileName = NULL;
    file->filePath = NULL;
    file->fileSize = NULL;
}

int isRelativeDirPath(char *value){
    if(value[0] == '.'){
        if((value[1] == '\0') || (value[1] == '.' && value[2] == '\0')){
            return 1;
        }
    }
    return 0;
};

char *getMD5Hash(char *fileName) {
    char *fileText = NULL;
    int fileSize = 0;
    char digest[16];
    char *hash = malloc(sizeof(char)*33);

    FILE *fp = fopen(fileName, "r");
    if(fp != NULL) {
        fseek(fp, 0, SEEK_END);
        fileSize = ftell(fp);
        rewind(fp);

        fileText = (char*) malloc(sizeof(char) * fileSize);
        fread(fileText, 1, fileSize, fp);

        fclose(fp);
    }

    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx,fileText,fileSize);
    MD5_Final(digest,&ctx);
    int i;
    for(i = 0; i < 16;i++){
        sprintf(&hash[i*2],"%02x",(unsigned int)digest[i]);
    }

    return hash;
}

//
void readDir(char *string) {
    DIR *dir;
    if((dir = opendir(string)) != NULL){
        struct dirent *dirp;
        while((dirp = readdir(dir)) != NULL){
            char *currDir = malloc(sizeof(char) * 255 );
            sprintf(currDir, "%s/%s", string, dirp->d_name);
            if(dirp->d_type == DT_DIR && !isRelativeDirPath(dirp->d_name)){
                readDir(currDir);
            } else if (dirp->d_type != DT_DIR) {
                struct stat lstatAction;
                lstat(currDir, &lstatAction);
                if(lstatAction.st_mode != S_IFLNK) {
                    char *md5HashValue = getMD5Hash(currDir);
                    fileListAppend(currDir, lstatAction.st_size, lstatAction.st_mtim, md5HashValue,dirp->d_name);
                }
            }
        }
        closedir(dir);
    }
}

void fileListAppend(char *filePath, unsigned *fileSize, struct timespec lastEdited, char *md5HashValue, char *fileName) {
    file *node = malloc(sizeof(file));
    setFileValues(node);
    node->filePath = filePath;
    node->fileSize = fileSize;
    node->fileName = fileName;
    node->lastEdited = lastEdited;
    node->fileMD5Hash = md5HashValue;
    if(first == NULL){
        first = node;
        last = node;
    } else if(first == last){
        last = node;
        first->next = node;
        last->prev = first;
    } else {
        last->next = node;
        node ->prev = last;
        last = node;
    }
}


void printDirs(){
    file *curr;
//    for ()
    for (curr = first; curr != NULL; curr = curr->next) {
//        printf("%d %d %s %s %s\n",curr->fileSize,curr->lastEdited.tv_nsec, curr->fileMD5Hash ,curr->filePath, curr->fileName);
        printf("%s %s\n",curr->filePath, curr->fileName);
//        printf("%s\n",curr->filePath);
    }
}

int isdFlag(char *string){
    if(string[0] == '-' && string[1] == 'd' && string[2] == '\0'){
        return 1;
    } else return 0;

};

int ismFlag(char *string){
    if(string[0] == '-' && string[1] == 'm' && string[2] == '\0'){
        return 1;
    } else return 0;

};

int ishFlag(char *string){
    if(string[0] == '-' && string[1] == 'h' && string[2] == '\0'){
        return 1;
    } else return 0;

};

void printHelp(){
    printf("bez papildus 'karogiem' salidzina faila nosaukumu un izmeru");
    printf("-d parbauda ari faila satura izmainu datumu (st_mtime) sakritibu.\n");
    printf("-m aprekina un salidzina MD5 vertibu faila saturam (bez varda un datuma).\n");
    printf("-h izvada paliga tekstu par parametriem\n");
}

void printUnique(int state){
    file *outerLoop;
    file *innerLoop;
    file *temp;
    int foundEqual;
    for(outerLoop = first;outerLoop != NULL; outerLoop = outerLoop->next){
        foundEqual = 0;
        for(innerLoop = first->next; innerLoop != NULL; innerLoop = innerLoop->next){
            switch(state){
                case 0:
                    if(
//                            strcmp(&outerLoop->fileName,&innerLoop->fileName) ==0
//                       && outerLoop->fileSize == innerLoop->fileSize){
                       outerLoop->fileSize == innerLoop->fileSize){
                        if (foundEqual == 1) {
                            printf("%s\n",outerLoop->filePath);
                            if(innerLoop->next != NULL){
                                innerLoop->prev->next = innerLoop->next;
                                innerLoop->next->prev = innerLoop->prev;
                                innerLoop = innerLoop->prev;
                            } else {
                                innerLoop->prev->next = NULL;
                            }
                        }
                        if(foundEqual == 0){
                            printf("=======================\n");
                            printf("%s\n", innerLoop->filePath);
                            foundEqual = 1;
                        }
//                        temp = innerLoop->prev;
                    }
                    break;

                case 1:
                    if(strcmp(&outerLoop->fileName,&innerLoop->fileName) == 0
                       && outerLoop->fileSize == innerLoop->fileSize
                       && outerLoop->lastEdited.tv_sec == innerLoop->lastEdited.tv_sec
                       && outerLoop->lastEdited.tv_nsec == innerLoop->lastEdited.tv_nsec){
                        printf("%s\n",innerLoop->filePath);
                    }
                    break;

                case 2:
                    if(strcmp(&outerLoop->fileMD5Hash,&innerLoop->fileMD5Hash) == 0){
                        printf("%s\n",innerLoop->filePath);
                    }
                    break;

                case 3:
                    if(strcmp(&outerLoop->fileName,&innerLoop->fileName) == 0
                       && outerLoop->fileSize == innerLoop->fileSize
                       && outerLoop->lastEdited.tv_sec == innerLoop->lastEdited.tv_sec
                       && outerLoop->lastEdited.tv_nsec == innerLoop->lastEdited.tv_nsec
                       && strcmp(&outerLoop->fileMD5Hash,&innerLoop->fileMD5Hash) == 0){
                        printf("%s\n",innerLoop->filePath);
                    }
                    break;
            }
        }
    }
}

//    States:
//    0 : name/size
//    1 : name/size/date
//    2 : MD5 hash
//    3 : MD5 hash/name/size/date

int main(int argc, char *argv[]) {
    readDir(".");
    int i;
    int containsD = 0;
    int containsM = 0;

    if(argc > 1){
        for(i=1 ; i<argc; i++){
            if(ishFlag(argv[i])) {
                printHelp();
                return 0;
            }
            if(isdFlag(argv[i])) {
                containsD = 1;
            }
            if(ismFlag(argv[i])) {
                containsM = 1;
            }
        }

        if(containsM && containsD){
            printUnique(3);
        } else if(containsD) {
            printUnique(1);
        } else if(containsM) {
            printUnique(2);
        } else {
            printHelp();
        }
    } else {
//        printUnique(0);
        printDirs();
        return 0;
    }
    return 0;
}

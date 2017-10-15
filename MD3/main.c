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

typedef struct identicalFileList{
    file *first;
    file *last;

    struct identicalFileList *next;
    struct identicalFileList *prev;
} identicalFileList;

identicalFileList *firstList = NULL;
identicalFileList *lastList = NULL;


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

void setFileListValues(identicalFileList *node){
    node->next=NULL;
    node->first=NULL;
    node->last=NULL;
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
                struct stat statAction;
                stat(currDir, &statAction);
                char *md5HashValue = getMD5Hash(currDir);
                fileListAppend(currDir, statAction.st_size, statAction.st_mtim, md5HashValue,dirp->d_name);
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
        printf("%d %d %s %s\n",curr->fileSize,curr->lastEdited.tv_nsec, curr->fileMD5Hash ,curr->filePath);
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

file *createFileCopy(file *node){
    file *newNode = malloc(sizeof(file));
//    memcpy(newNode, node, sizeof(file));
    newNode->fileMD5Hash = node->fileMD5Hash;
    newNode->lastEdited = node->lastEdited;
    newNode->fileSize = node->fileSize;
    newNode->fileName = node->fileName;
    newNode->filePath = node->filePath;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

void addItemToList(identicalFileList *list, file *node){
    if(list->first == NULL){
        list->first = node;
        list->last = node;
    } else if(list->first == list->last){
        list->last = node;
        list->first->next = node;
        list->last->prev = list->first;
    } else {
        list->last->next = node;
        node->prev = list->last;
        list->last = node;
    }
}

identicalFileList *findIdenticalFileList(file *fileNode, int state){
    identicalFileList *currList;
    for(currList = firstList;currList != NULL; currList = currList->next){
        switch(state){
            case 0:
                if(currList->first->fileName == fileNode->fileName
                   && currList->first->fileSize == fileNode->fileSize){
                    return currList;
                }
                break;

            case 1:
                if(currList->first->fileName == fileNode->fileName
                   && currList->first->fileSize == fileNode->fileSize
                   && currList->first->lastEdited.tv_sec == fileNode->lastEdited.tv_sec){
                    return currList;
                }
                break;

            case 2:
                if(currList->first->fileMD5Hash == fileNode->fileMD5Hash){
                    return currList;
                }
                break;

            case 3:
                if(currList->first->fileName == fileNode->fileName
                   && currList->first->fileSize == fileNode->fileSize
                   && currList->first->lastEdited.tv_sec == fileNode->lastEdited.tv_sec
                   && currList->first->fileMD5Hash == fileNode->fileMD5Hash){
                    return currList;
                }
                break;
        }
    }
    return NULL;
}

void processFiles(int state){
//    States:
//    0 : name/size
//    1 : name/size/date
//    2 : MD5 hash
//    3 : MD5 hash/name/size/date
    //                    addItemToList(currList, fileNode);
    file *fileNode;
    for(fileNode=first;fileNode != NULL;fileNode=fileNode->next){
        identicalFileList *list = findIdenticalFileList(fileNode,state);
        if(list != NULL) {
            file *passedNode = createFileCopy(fileNode);
            addItemToList(list,passedNode);
        } else {
            identicalFileList *fileList;
            fileList = malloc(sizeof(fileList));
            setFileListValues(fileList);
            if(firstList == NULL ){
                firstList = fileList;
                lastList = fileList;
                file *passedNode = createFileCopy(fileNode);
                addItemToList(list,passedNode);
            } else if (firstList == lastList){
                firstList->next = fileList;
                lastList = fileList;
                lastList->prev = firstList;
                file *passedNode = createFileCopy(fileNode);
                addItemToList(list,passedNode);
            } else {
                lastList->next = fileList;
                fileList->prev = lastList;
                lastList = fileList;
                file *passedNode = createFileCopy(fileNode);
                addItemToList(list,passedNode);
            }
        }
    }
}

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
            processFiles(3);
        } else if(containsD) {
            processFiles(1);
        } else if(containsM) {
            processFiles(2);
        } else {
            printHelp();
        }
    } else {
        processFiles(0);
        return 0;
    }
    return 0;
}

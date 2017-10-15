#include <stdio.h>
#include <memory.h>
#include <dirent.h>
#include <stdlib.h>

int process_directory(char *filename, char *path) {
    char slash = '/';
    DIR* dir;
    struct dirent *entry;
    char *NulPosition = &path[strlen(path)];
    if ((dir = opendir(path)) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            if(strcmp(filename,entry->d_name) == 0){
            } 
            if(path[strlen(path)-1] == slash){
                printf("%s%s\n", path, entry->d_name);
            } else {
                printf("%s%c%s\n", path, slash, entry->d_name);
            }
            if (entry->d_type == DT_DIR) {
                if ((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0)) {
                    sprintf(NulPosition, "%c%s", slash, entry->d_name);
                    if (process_directory(path,filename)) {
                        closedir(dir);
                        return 1;
                    }
                    *NulPosition = '\0';
                }
            }
        }
    }
    closedir(dir);
    return 0;
}

int list_directories(char *filename, char *path){
    struct dirent *entry;
    char word_length[256+sizeof(entry->d_name)+1];
    if (strlen(path) > 255){
        return 1;
    }
    return process_directory(filename,strcpy(word_length, path));
}

int main(int argc, char *argv[]) {
    if(argv[1] == NULL || argv[2] == NULL){
        return -1;
    }
    list_directories(argv[1],argv[2]);
    return 0;
}

//void PrintFileDirs(const char *path,char *file_name)
//{
//    DIR *dir;
//    struct dirent *entry;
//    dir = opendir(path);
//    if (!dir){
//        return;
//    }
//
//    while ((entry = readdir(dir)) != NULL) {
//        if (entry->d_type == DT_DIR) {
//            char path[1024];
//            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
//                continue;
//            }
////            snprintf(path, sizeof(path), "%s/%s", path, entry->d_name);
//            printf("%s %s\n",path, entry->d_name);
//            PrintFileDirs(entry->d_name,file_name);
//        } else {
//            printf("%s%s\n",path, entry->d_name);
//        }
//    }
//    closedir(dir);
//}
//
//int main(int argc, char *argv[]) {
//    PrintFileDirs("/tmp/","*");
//    return 0;
//}

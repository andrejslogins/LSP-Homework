#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>

int word_length = 64;


typedef struct lineArgs{
    char *type;
    char *name;
} lineArgs;

typedef struct familyNode {
    char *name;
    char *father;
    char *mother;

    struct familyNode *prev;
    struct familyNode *next;
} familyNode;

typedef struct familyTreeNode {
    struct familyNode *familyNodeList;
    struct familyTreeNode *next;
} familyTreeNode;

typedef struct familyTreeNodeAndFamilyNode {
    familyNode *familyNode;
    familyTreeNode *familyTree;
} familyTreeNodeAndFamilyNode;


void setfamilyNodeDefaults(familyNode *node) {
    node = malloc(sizeof(familyNode));
    node->next = NULL;
    node->father = NULL;
    node->mother = NULL;
    node->prev = NULL;
}

void removeSpaces(const char *input, char *result) {
    int i, j = 0;
    for (i = 0; input[i] != '\0'; i++) {
        if (!isspace((unsigned char) input[i])) {
            result[j++] = input[i];
        }
    }
    result[j] = '\0';
}

int isWordEmpty(char *word){
    int size = getWordLength(word);
    if (size>0) return 0;
    else return 1;
}

lineArgs Get_Line_Args(char line[word_length]) {
    lineArgs *Line_Args;
    char tmp[word_length];
    Line_Args = malloc(sizeof(Line_Args));
    int type_pos = 0;
    int name_pos = 0;
    char *type;
    char *name;
    for (int i = 0; i < word_length; ++i) {
        if (type_pos != -1) {
            if (isspace(line[i])) {
                tmp[type_pos]='\0';
                type = malloc((getWordLength(tmp)) * sizeof(char));
                removeSpaces(tmp, type);
                type_pos = -1;
            } else {
                tmp[type_pos] = line[i];
                type_pos++;
            }
        } else if (name_pos != -1) {
            if (isspace(line[i])) {
                tmp[name_pos]='\0';
                name = malloc((getWordLength(tmp)+1) * sizeof(char));
                removeSpaces(tmp, name);
                name_pos = -1;
            } else {
                tmp[name_pos] = line[i];
                name_pos++;
            }
        } else {
            Line_Args->name = name;
            Line_Args->type = type;
            return *Line_Args;
        }
    }
}

int wordsEqual(char *word1, char *word2){
    return strcmp(word1,word2);
}

int getWordLength(char *word){
    for(int i=0; i<word_length;i++){
        if(isspace(word[i])){
            return i;
        }
    }
    return 0;
}


//familyNodeList *family_node_list_last;
//familyNodeList *family_node_list_first;
//familyNode *get_node(familyNode *root,char *name){
//    if(wordsEqual(root->name, name)) {
//        return root;
//    } else if(root->father != NULL) {
//        return get_node(root->father,name);
//    } else if(root->mother != NULL) {
//        return get_node(root->mother, name);
//    } else {
//        return NULL;
//    }
//}


//familyNode *findNode(char *name) {
//    familyNode *node;
//    for (familyNodeList *elem = family_node_list_first; elem != NULL; elem = elem->next) {
//        node = get_node(elem->curr, name);
//        if(node != NULL) {return node;}
//    }
//    return NULL;
//void printNode(familyNode *pNode){
//
//void printTree(){
//    for(familyNodeList *curr = family_node_list_first; curr != NULL; curr = curr->next){
//        printNode(curr->curr);
//    }
//}

//void familyNodeListAdd(familyNode *node);

//}

//}


int readWriteFile(char *in_filename, char *out_filename){
    char line[word_length];
    lineArgs Line_Args;
    FILE *in, *out;
    in = fopen(in_filename,"r");
    if (in){
        while(fgets(line,sizeof(char)*word_length,in)){
            Line_Args = Get_Line_Args(line);
            ProcessLine(&Line_Args);
        }
        fclose(in);
    } else {
        return 0;
    }
    return 0;
}

void ProcessLine(lineArgs *Line_Args) {
    if(wordsEqual("VARDS", (*Line_Args).type) ) {
        familyNode *node = malloc(sizeof(familyNode));
        setfamilyNodeDefaults(node);
        node->name = (*Line_Args).name;
    }
//    if((*Line_Args).type[0] != '\0' && (*Line_Args).name[0] != '\0'){
//        if(wordsEqual("MATE", (*Line_Args).type)) {
//            mother_name = (*Line_Args).name;
//        }
//        if(wordsEqual("TEVS", (*Line_Args).type)) {
//            father_name = (*Line_Args).name;
//        }
//    } else {
//        familyNode *node;
//        setNodeDefaults(node);
//        node->name = self_name;
//        node->father = findNode(father_name);
//        if(node->father ==NULL && isWordEmpty(father_name)){
//            familyNode *father = malloc(sizeof(familyNode));
//            father->name=father_name;
//            familyNodeListAdd(father);
//        }
//        node->mother = findNode(mother_name);
//        if(node->mother == NULL && isWordEmpty(mother_name)){
//            familyNode *mother = malloc(sizeof(familyNode));
//            mother->name=mother_name;
//            familyNodeListAdd(mother);
//        }
//        if(node->mother == NULL && node->father == NULL){
//            familyNodeListAdd(node);
//        }
//        self_name = "";
//        father_name = "";
//        mother_name = "";
//    }
}




void familyNodeListAdd(familyNode *node) {
//    if(family_node_list_first == NULL){
//        family_node_list_first = malloc(sizeof(familyNodeList));
//        family_node_list_first->curr = node;
//        family_node_list_last = family_node_list_first;
//    }
//    else if(family_node_list_first == family_node_list_last){
//        family_node_list_first->next = malloc(sizeof(familyNodeList));
//        family_node_list_last= family_node_list_first->next;
//        family_node_list_first->next->curr = node;
//    } else {
//        family_node_list_last->next = malloc(sizeof(familyNodeList));
//        family_node_list_last = family_node_list_last->next;
//        family_node_list_last->curr = node;
//    }
}

int main(int argc, char *argv[]) {
    if(argv[1] != "" && argv[2] != ""){
        return readWriteFile("file.in", "file.out");
    } else {
        return readWriteFile(argv[1], argv[2]);
    }
}

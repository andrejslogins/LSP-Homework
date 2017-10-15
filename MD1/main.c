#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>


// Andrejs Logins al13005
// Rindinu lasisana norealizeta, talaka darbiba diemzel nav realizeta

int word_length = 64;


struct line_args{
    char *type;
    char *name;
};

struct family_node {
    char *name;
    struct family_node *father;
    struct family_node *mother;
};

char *self_name;
char *father_name;
char *mother_name;

struct family_node_list {
    struct family_node *curr;
    struct family_node_list *next;
};

struct family_node_list *family_node_list_first;
struct family_node_list *family_node_list_last;

struct line_args Get_Line_Args(char line[word_length]) {
    struct line_args *Line_Args;
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
                type = malloc((get_word_length(tmp)) * sizeof(char));
                remove_spaces(tmp,type);
                type_pos = -1;
            } else {
                tmp[type_pos] = line[i];
                type_pos++;
            }
        } else if (name_pos != -1) {
            if (isspace(line[i])) {
                tmp[name_pos]='\0';
                name = malloc((get_word_length(tmp)+1) * sizeof(char));
                remove_spaces(tmp,name);
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

struct family_node *get_node(struct family_node *root,char *name){
    if(words_equal(root->name,name)) {
        return root;
    } else if(root->father != NULL) {
        return get_node(root->father,name);
    } else if(root->mother != NULL) {
        return get_node(root->mother, name);
    } else {
        return NULL;
    }
}

int get_word_length(char *word){
    for(int i=0; i<word_length;i++){
        if(isspace(word[i])){
            return i;
        }
    }
    return 0;
}

int is_word_empty(char *word){
    int size = get_word_length(word);
    if (size>0) return 0;
    else return 1;
}
void remove_spaces(const char *input, char *result)
{
    int i, j = 0;
    for (i = 0; input[i] != '\0'; i++) {
        if (!isspace((unsigned char) input[i])) {
            result[j++] = input[i];
        }
    }
    result[j] = '\0';
}

struct family_node *find_node(char *name) {
    struct family_node *node;
    for (struct family_node_list *elem = family_node_list_first; elem != NULL; elem = elem->next) {
        node = get_node(elem->curr, name);
        if(node != NULL) {return node;}
    }
    return NULL;
}


int main(int argc, char *argv[]) {
    if(argv[1] != "" && argv[2] != ""){
        return Read_Write_File("file.in", "file.out");
    } else {
        return Read_Write_File(argv[1], argv[2]);
    }
}


int Read_Write_File(char *in_filename, char *out_filename){
    char line[word_length];
    struct line_args Line_Args;
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

void ProcessLine(struct line_args *Line_Args) {
    if((*Line_Args).type[0] != '\0' && (*Line_Args).name[0] != '\0'){
        if(words_equal("MATE", (*Line_Args).type)) {
            mother_name = (*Line_Args).name;
        }
        if(words_equal("TEVS", (*Line_Args).type)) {
            father_name = (*Line_Args).name;
        }
        if(words_equal("VARDS", (*Line_Args).type) ) {
            self_name = (*Line_Args).name;
        }
    } else {
        struct family_node *node = malloc(sizeof(struct family_node));
        node->name = self_name;
        node->father = find_node(father_name);
        if(node->father ==NULL && is_word_empty(father_name)){
            struct family_node *father = malloc(sizeof(struct family_node));
            father->name=father_name;
            Family_Node_List_Add(father);
        }
        node->mother = find_node(mother_name);
        if(node->mother == NULL && is_word_empty(mother_name)){
            struct family_node *mother = malloc(sizeof(struct family_node));
            mother->name=mother_name;
            Family_Node_List_Add(mother);
        }
        if(node->mother == NULL && node->father == NULL){
            Family_Node_List_Add(node);
        }
        self_name = "";
        father_name = "";
        mother_name = "";
    }
}

void Family_Node_List_Add(struct family_node *node) {
    if(family_node_list_first == NULL){
        family_node_list_first = malloc(sizeof(struct family_node_list));
        family_node_list_first->curr = node;
        family_node_list_last = family_node_list_first;
    }
    else if(family_node_list_first == family_node_list_last){
        family_node_list_first->next = malloc(sizeof(struct family_node_list));
        family_node_list_last= family_node_list_first->next;
        family_node_list_first->next->curr = node;
    } else {
        family_node_list_last->next = malloc(sizeof(struct family_node_list));
        family_node_list_last = family_node_list_last->next;
        family_node_list_last->curr = node;
    }
}

int words_equal(char *word1 ,char *word2){
    return strcmp(word1,word2);
}

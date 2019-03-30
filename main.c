#include <stdio.h>
#include<stdlib.h>
#include <string.h>

#define Boolean char
#define TRUE 1
#define FALSE 0


enum Data_Type{INTEGER, FLOAT, STRING,LIST };

struct Object{
    union Data{
        int Integer;
        float Float;
        char* String;
        struct LinkedList* list;
    }data;
    enum Data_Type type;
};

struct Node{
    struct Object value;
    struct Node* next;
    //struct Node* previous;
};


struct LinkedList{
    struct Node* head;
    struct Node* tail;
    int length;
};

void combineListToStringHelper(struct LinkedList* list, char* string);
void printList(struct LinkedList*);


void printNode (struct Node* node){
    if(node == NULL){
        return;
    }
    switch (node->value.type) {
        case INTEGER: {
            printf("%d,", node->value.data.Integer);
            break;
        }
        case FLOAT: {
            printf("%f,", node->value.data.Float);
            break;
        }
        case STRING: {
            printf("%s,", node->value.data.String);
            break;
        }
        case LIST: {
            printList(node->value.data.list);
            break;
        }
        default: {
            printf("Error！");
            break;
        }
    }
}

void printList(struct LinkedList *list){
    struct Node* pointer = list->head;
    while(pointer != NULL){
        printNode(pointer);
        pointer = pointer->next;
    }
    printf("\n");
    return;
}

void selectByIndex(struct LinkedList* list, int index){
    struct Node* pointer = list->head;
    int i = 0;
    while(pointer != NULL && i < index){
        pointer = pointer->next;
        i++;
    }
    struct Object res = pointer->value;
    switch (res.type){
        case INTEGER:{
            printf("%d,", res.data.Integer);
            break;
        }
        case FLOAT:{
            printf("%f,", res.data.Float);
            break;
        }
        case STRING:{
            printf("%s,", res.data.String);
            break;
        }
        case LIST:{
            printList(res.data.list);
            break;
        }
        default:{
            printf("Error！");
            break;
        }
    }
}

void modifyByIndex(struct LinkedList* list,int index, struct Object value){
    struct Node* pointer = list->head;
    int i = 0;
    while(pointer != NULL && i < index){
        pointer = pointer->next;
        i++;
    }
    pointer->value = value;
    return;
}

void appendToList(struct LinkedList* list,struct Object object){
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->value = object;
    //newNode->previous = list->tail;
    newNode->next = NULL;
    list->tail->next = newNode;
    list->tail = newNode;
    list->length++;
    return;
}

int length(struct LinkedList list){return list.length;}

void insertByIndex(struct LinkedList* list, int index, struct Object object){
    struct Node* pointer = list->head;
    int i = 0;
    while(pointer != NULL && i < index - 1){
        pointer = pointer->next;
    }
    if(pointer != NULL){
        struct Node* newNode = malloc(sizeof(struct Node));
        newNode->value = object;
        newNode->next = pointer->next;
        pointer->next = newNode;
        list->length++;
    }else{
        printf("Index out of bound!\n");
    }

    return;
}

void delete(struct LinkedList* list){
    free(list);
    free(list->head);
    if(list->head != NULL){
        struct Node* pointer = list->head->next;
        while(pointer != NULL){
            free(pointer);
            list->head = pointer;
            pointer = pointer->next;
        }
    }
    free(list->tail);

}

char* toStringHelper(struct Node* node){
    char* string = malloc(100*sizeof(char));
    switch (node->value.type){
        case INTEGER:{
           // itoa(node->value.data.Integer,string, 10);
            sprintf(string,"%d", node->value.data.Integer);
            break;
        }
        case FLOAT:{
            sprintf(string,"%f", node->value.data.Float);
            break;
        }
        case STRING:{
            string = node->value.data.String;
        }
        case LIST:{
            combineListToStringHelper(node->value.data.list, string);
            break;
        }
    }
    return string;
}
void combineListToStringHelper(struct LinkedList* list, char* string){
    struct Node* pointer = list->head;
    while(pointer != NULL){
        char* stringTemp = toStringHelper(pointer);
        strcat(string, stringTemp);
        pointer = pointer->next;
    }
    return;
}
struct Node* min(struct LinkedList* list){
    if(list->head == NULL){
        return NULL;
    }
    char* minString = toStringHelper(list->head);
    struct Node* minNode = list->head;
    struct Node* pointer = list->head->next;
    while(pointer != NULL){
        char* string = toStringHelper(pointer);
        if(strcmp(minString, string) > 0){
           minString = string;
           minNode = pointer;
        }
        pointer = pointer->next;
    }
    return minNode;
}

struct Node* max(struct LinkedList* list){
    if(list->head == NULL){
        return NULL;
    }
    char* maxString = toStringHelper(list->head);
    struct Node* maxNode = list->head;
    struct Node* pointer = list->head->next;
    while(pointer != NULL){
        char* string = toStringHelper(pointer);
        if(strcmp(maxString, string) < 0){
            maxString = string;
            maxNode = pointer;
        }
        pointer = pointer->next;
    }
    return maxNode;
}

void concat(struct LinkedList* listOne, struct LinkedList* listTwo){
    listOne->tail->next = listTwo->head;
    listOne->tail = listTwo->tail;
    listOne->length += listTwo->length;
    return;
}

void reverse(struct LinkedList* list){
    if(list->head == NULL){
        return;
    }
    list->tail = list->head;
    struct Node* pointer = list->head->next;
    list->head->next = NULL;
    while(pointer != NULL){
        struct Node* extraPointer = pointer->next;
        pointer->next = list->head;
        list->head = pointer;
        pointer = extraPointer;
    }
    return;
}


struct Node* getPreviousNodeOfMinHelper(struct LinkedList* list){
    if(list->head == NULL){
        return NULL;
    }
    char* minString = toStringHelper(list->head);
    struct Node* previousNode = list->head;
    struct Node* pointer = list->head;
    while(pointer->next != NULL){
        char* string = toStringHelper(pointer->next);
        if(strcmp(minString, string) > 0){
            minString = string;
            previousNode= pointer;
        }
        pointer = pointer->next;
    }
    if(strcmp(toStringHelper(list->head), minString) <= 0){
        previousNode = malloc(sizeof(struct Node));
        previousNode->next = list->head;
    }
    return previousNode;
}

struct Node* getPreviousNodeOfMaxHelper(struct LinkedList* list){
    if(list->head == NULL){
        return NULL;
    }
    char* maxString = toStringHelper(list->head);
    struct Node* previousNode = list->head;
    struct Node* pointer = list->head;
    while(pointer->next != NULL){
        char* string = toStringHelper(pointer->next);
        if(strcmp(maxString, string) < 0){
            maxString = string;
            previousNode= pointer;
        }
        pointer = pointer->next;
    }
    if(strcmp(toStringHelper(list->head), maxString) <= 0){
        previousNode = malloc(sizeof(struct Node));
        previousNode->next = list->head;
    }
    return previousNode;
}

void sort(struct LinkedList* list, Boolean reverse){
    if(!reverse){
        struct Node* previousOfMinNode = NULL;
        struct Node* minNode = NULL;
        struct Node* newHead = list->head;
        while(list->head != NULL){
            previousOfMinNode = getPreviousNodeOfMinHelper(list);
            minNode = previousOfMinNode->next;
            if(minNode == list->head){
                list->head = list->head->next;
            }else{
                previousOfMinNode->next = minNode->next;
                minNode->next = newHead;
                newHead = minNode;
            }
            if(list->head !=NULL && list->head->next == NULL){
                list->tail = list->head;
            }
        }
        list->head = newHead;
        free(previousOfMinNode);
    }else{
        struct Node* previousOfMaxNode = NULL;
        struct Node* maxNode = NULL;
        struct Node* newHead = list->head;
        while(list->head != NULL){
            previousOfMaxNode = getPreviousNodeOfMaxHelper(list);
            maxNode = previousOfMaxNode->next;
            if(previousOfMaxNode->next == list->head){
                list->head = list->head->next;
            }else{
                previousOfMaxNode->next = maxNode->next;
                maxNode->next = newHead;
                newHead = maxNode;
            }
            if(list->head->next == NULL){
                list->tail = list->head;
            }
        }
        list->head = newHead;
        free(previousOfMaxNode);
    }
}

int main() {
    struct LinkedList *list = NULL;
    list = malloc(2*sizeof(struct Node));

    struct Node *head = malloc(sizeof(struct Node));
    struct Object *value_one = malloc(sizeof(struct Object));
    value_one->data.Integer= 1;
    value_one->type = INTEGER;
    head->value = *value_one;
    head->next = NULL;
   // head->previous = NULL;
    list->head = head;

    struct Node *tail = malloc(sizeof(struct Node));
    struct Object *value_two = malloc(sizeof(struct Object));
    value_two->data.Integer = 2;
    value_two->type = INTEGER;
    tail->value = *value_two;
    tail->next = NULL;
   // tail->previous = head;
    list->tail = tail;
    head->next = tail;

    struct Object *value_three = malloc(sizeof(struct Object));
    value_three->data.Integer = 3;
    value_three->type = INTEGER;
    appendToList(list, *value_three);
    printList(list);
    selectByIndex(list,1);
    reverse(list);
    printList(list);
    modifyByIndex(list,1,*value_three);
    printList(list);
    sort(list,FALSE);
    printList(list);
    printf("%d\n", min(list)->value.data.Integer);

    struct Object* value_four = malloc(10*sizeof(char));
    value_four->data.String = "Hello";
    value_four->type = STRING;
    insertByIndex(list,1,*value_four);
    printList(list);

    return 0;
}
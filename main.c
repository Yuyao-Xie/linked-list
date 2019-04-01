#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <ctype.h>

#define Boolean int
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
};


struct LinkedList{
    struct Node* head;
    struct Node* tail;
    int length;
};

void combineListToStringHelper(struct LinkedList* list, char* string);
void printList(struct LinkedList*);
struct Object* createObjectFromInput(char *inputValue);


void printNode (struct Node* node){
    if(node == NULL){
        return;
    }
    switch (node->value.type) {
        case INTEGER: {
            printf("%d", node->value.data.Integer);
            break;
        }
        case FLOAT: {
            printf("%f", node->value.data.Float);
            break;
        }
        case STRING: {
            printf("%s", node->value.data.String);
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

    if(pointer == NULL) {
        printf("List is empty!\n");
        return;
    }

    printf("[");
    while(pointer != NULL){
        printNode(pointer);
        if(pointer->next != NULL) {
            printf(",");
        }
        pointer = pointer->next;
    }
    printf("]");

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

void modify(struct LinkedList *list, int index, struct Object value){
    struct Node* pointer = list->head;
    int i = 0;
    while(pointer != NULL && i < index){
        pointer = pointer->next;
        i++;
    }
    pointer->value = value;
    return;
}

struct LinkedList* append(struct LinkedList *list, struct Object object){
    struct Node* newNode = malloc(sizeof(struct Node));

    newNode->value = object;
    newNode->next = NULL;

    if (list->tail == NULL) {

        list->head = newNode;
        list->tail = newNode;

    } else {
        list->tail->next = newNode;
        list->tail = newNode;

    }
    list->length++;
    return list;
}

int length(struct LinkedList list){return list.length;}

void insert(struct LinkedList *list, int index, struct Object object){
    struct Node *head = malloc(sizeof(struct Node));
    head->next = list->head;
    struct Node* pointer = head;
    int i = 0;
    while(pointer != NULL && i < index){
        pointer = pointer->next;
        i++;
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
    if(index == 0) {
        list->head = head->next;
    }
    free(head);
    return;
}

void delete(struct LinkedList* list){
    while(list->head != NULL){
        struct Node* pointer = list->head;
        list->head = list->head->next;
        if (pointer->value.type == STRING) {
            free(pointer->value.data.String);
        }
        if (pointer->value.type == LIST) {
            delete(pointer->value.data.list);
        }
        free(pointer);
    }
    free(list);
}

char* toStringHelper(struct Node* node){
    char* string = malloc(100*sizeof(char));
    switch (node->value.type){
        case INTEGER:{
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
    if(strcmp(toStringHelper(list->head), maxString) >= 0){
        previousNode = malloc(sizeof(struct Node));
        previousNode->next = list->head;
    }
    return previousNode;
}

void sort(struct LinkedList* list, Boolean reverse){
    struct Node* newHead = malloc(sizeof(struct Node));
    struct Node* newListPtr = newHead;
    if(!reverse){
        struct Node* previousOfMinNode = NULL;
        struct Node* minNode = NULL;
        while(list->head != NULL){
            previousOfMinNode = getPreviousNodeOfMinHelper(list);
            minNode = previousOfMinNode->next;
            newListPtr->next = minNode;
            newListPtr = minNode;
            if(minNode == list->head){
                list->head = list->head->next;
                free(previousOfMinNode);
            } else {
                previousOfMinNode->next = minNode->next;
            }
        }
    }else{
        struct Node* previousOfMaxNode = NULL;
        struct Node* maxNode = NULL;
        while(list->head != NULL){
            previousOfMaxNode = getPreviousNodeOfMaxHelper(list);
            maxNode = previousOfMaxNode->next;
            newListPtr->next = maxNode;
            newListPtr = newListPtr->next;
            if(maxNode == list->head){
                list->head = list->head->next;
                free(previousOfMaxNode);
            }else{
                previousOfMaxNode->next = maxNode->next;
            }
        }
    }
    list->head = newHead->next;
    list->tail = newListPtr;
    free(newHead);
}

void printMenu() {
    printf("Python Linked List Menu\n");
    printf("   Selected the following options\n");
    printf("\ta: Print linked list\n");
    printf("\tb: Print length of linked list\n");
    printf("\tc: Append to linked list\n");
    printf("\td: Modify linked list at index\n");
    printf("\te: Insert to linked list at index\n");
    printf("\tf: Find min element in linked list\n");
    printf("\tg: Find max element in linked list\n");
    printf("\th: Concat two linked list\n");
    printf("\ti: Reverse linked list\n");
    printf("\tj: Sort linked list\n");
    printf("\tk: Sort linked list in reverse\n");
    printf("\tl: Delete linked list\n");
}

struct LinkedList* createList() {
    struct LinkedList* list = malloc(sizeof(struct LinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    return list;
}

struct LinkedList* convertStringToList(char *inputStr, int len)
{
    struct LinkedList *innerlist = createList();

    char * pch;
    inputStr[len-1] = '\0';
    inputStr++;

    pch = strtok (inputStr,",");

    while (pch != NULL)
    {
//        printf ("%s\n", pch);
        append(innerlist, *createObjectFromInput(pch));
        pch = strtok (NULL, ",");
    }

    return innerlist;
}

struct Object* createObjectFromInput(char *inputValue) {
    struct Object *value = malloc(sizeof(struct Object));
    int length = strlen(inputValue);
    Boolean isFloat = FALSE;
    Boolean isString = FALSE;
    Boolean isList = FALSE;

    if(inputValue[0] == '[' && inputValue[length-1] == ']') {
        isList = TRUE;
    }
    else {
        for (int i = 0; i< length; i++) {
            if (inputValue[i] == '.' && !isFloat) {
                isFloat = TRUE;
            } else if (!isdigit(inputValue[i])) {
                isString = TRUE;
                break;
            }
        }
    }

    if (isString) {
        value->type = STRING;
        value->data.String = strcpy(malloc((strlen(inputValue) + 1) * sizeof(char)), inputValue);
    } else if (isFloat) {
        value->type = FLOAT;
        value->data.Float = atof(inputValue);
    } else if (isList) {
        value->type = LIST;
        value->data.list = convertStringToList(inputValue, length);
    } else {
        value->type = INTEGER;
        value->data.Integer = atoi(inputValue);
    }
    return value;
}

int main() {
    struct LinkedList *list = createList();

    char option;
    char *inputValue = malloc(128 * sizeof(char));
    int index;
    while(1) {
        printMenu();

        gets(inputValue);
        if (strlen(inputValue) == 0 || strlen(inputValue) > 1) {
            printf("Please input valid option.\n");
            continue;
        }
        option = inputValue[0];
        switch (option) {
            case 'a':
                printList(list);
                printf("\n");
                break;
            case 'b':
                printf("The length of list is %d.\n", length(*list));
                break;
            case 'c':
                printf("Please input the value you want.\n");
                gets(inputValue);
                append(list, *createObjectFromInput(inputValue));
                break;
            case 'd':
                printf("Enter a index you want to modify in the Linked List.\n");
                scanf(" %d", &index);
                gets(inputValue);
                printf("Please input the value you want.\n");
                gets(inputValue);
                modify(list, index, *createObjectFromInput(inputValue));
                break;
            case 'e':
                printf("Enter a index you want to insert in the Linked List.\n");
                scanf(" %d", &index);
                gets(inputValue);
                printf("Please input the value you want.\n");
                gets(inputValue);
                insert(list, index, *createObjectFromInput(inputValue));
                break;
            case 'f':
                printf("Min Node: ");
                printNode(min(list));
                printf("\n");
                break;
            case 'g':
                printf("Max Node: ");
                printNode(max(list));
                printf("\n");
                break;
            case 'h':
                printf("Please input the list you want.\n");
                gets(inputValue);
                concat(list, convertStringToList(inputValue, strlen(inputValue)));
                break;
            case 'i':
                reverse(list);
                break;
            case 'j':
                sort(list, FALSE);
                break;
            case 'k':
                sort(list, TRUE);
                break;
            case 'l':
                delete(list);
                list = createList();
                break;
            default:
                return 0;
        }
    }
}

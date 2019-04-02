// Yuyao Xie
// Lacee Xu
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

void printNode (struct Node* node);
void printList(struct LinkedList*);
struct Object* selectByIndex(struct LinkedList* list, int index);
Boolean modify(struct LinkedList *list, int* indices, int length, struct Object value);
struct LinkedList* append(struct LinkedList *list, struct Object object);
int length(struct LinkedList list);
void insert(struct LinkedList *list, int index, struct Object object);
void delete(struct LinkedList* list);
char* toStringHelper(struct Node* node);
void combineListToStringHelper(struct LinkedList* list, char* string);
struct Node* min(struct LinkedList* list);
struct Node* max(struct LinkedList* list);
void concat(struct LinkedList* listOne, struct LinkedList* listTwo);
void reverse(struct LinkedList* list);
struct Node* getPreviousNodeOfMaxHelper(struct LinkedList* list);
void sort(struct LinkedList* list, Boolean reverse);
struct Object* createObjectFromInput(char *inputValue);
void printMenu();
struct LinkedList* createList();
struct LinkedList* convertStringToList(char *inputStr, int len);

/*
 * Function: printNode
 * ----------------------------
 *   Print a single node
 *
 *   node: Node ptr to the node to be printed
 */
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
            printf("\"%s\"", node->value.data.String);
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

/*
 * Function: printObject
 * ----------------------------
 *   Print a Object struct from it different data types
 *
 *   object: struct Object *object ptr to be printed
 */
void printObject(struct Object *object) {
    if(object == NULL){
        printf("Value is not found!");
        return;
    }
    switch (object->type) {
        case INTEGER: {
            printf("%d", object->data.Integer);
            break;
        }
        case FLOAT: {
            printf("%f", object->data.Float);
            break;
        }
        case STRING: {
            printf("%s", object->data.String);
            break;
        }
        case LIST: {
            printList(object->data.list);
            break;
        }
        default: {
            printf("Error！");
            break;
        }
    }
}

/*
 * Function: printList
 * ----------------------------
 *   Print the entire list starting from head
 *
 *   list: LinkedList ptr to be printed
 */
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

/*
 * Function: getValue
 * ----------------------------
 *   Return the value at a given index
 *
 *   list: struct LinkedList* ptr
 *   index: integer index of object to be returned
 *   length: int length of the linked list
 */
struct Object* getValue(struct LinkedList* list, int* indices, int length) {
    for (int i = 0; i<length - 1; i++){
        struct Object *value = selectByIndex(list, indices[i]);
        if (value == NULL || value->type != LIST) {
            return NULL;
        }
        list = value->data.list;
    }
    return selectByIndex(list, indices[length - 1]);
}

/*
 * Function: selectByIndex
 * ----------------------------
 *   Print the element a given index
 *
 *   list: LinkedList ptr to be looped to index
 *   index: integer idx
 */
struct Object* selectByIndex(struct LinkedList* list, int index){
    struct Node* pointer = list->head;
    int i = 0;
    while(pointer != NULL && i < index){
        pointer = pointer->next;
        i++;
    }
    return &pointer->value;
}

/*
 * Function: modify
 * ----------------------------
 *   Modify elem in the linked list given index and object.
 *
 *   list: LinkedList ptr to be looped to index
 *   index: int index in the linked list to be modified
 *   value: Object elment to replace the current element
 */
Boolean modify(struct LinkedList *list, int* indices, int length, struct Object value){
    struct Object *object = getValue(list, indices, length);
    if (object == NULL) {
        return FALSE;
    }
    *object = value;
    return TRUE;
}

/*
 * Function: append
 * ----------------------------
 *   Append a new elem to end of list.
 *
 *   list: List ptr to be appended
 *   object: Object elment to be appened
 *
 *   returns: the pointer to linkedlist
 */
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

/*
 * Function: length
 * ----------------------------
 *   Returns the length of the list
 *
 *   list: Linked List ptr
 *
 *   returns: int length of the list
 */
int length(struct LinkedList list){
    return list.length;
}

/*
 * Function: insert
 * ----------------------------
 *   Insert new Object at index of list.
 *
 *   list: struct LinkedList * ptr
 *   index: index index to be inserted
 *   object: Object to be inserted
 */
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

/*
 * Function: delete
 * ----------------------------
 *   Delete the linked list and free the memory.
 *
 *   list: struct LinkedList * ptr to be deleted
 */
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

/*
 * Function: toStringHelper
 * ----------------------------
 *   Helper function to convert Node* to string
 *
 *   node: struct Node* to be converted to string
 *
 *   return: char* str that's converted
 */
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

/*
 * Function: combineListToStringHelper
 * ----------------------------
 *   Helper function convert LinkedList to char* str
 *
 *   list: struct LinkedList* to be converted to string
 *   string: char* str ptr that represents the list
 */
void combineListToStringHelper(struct LinkedList* list, char* string){
    struct Node* pointer = list->head;
    while(pointer != NULL){
        char* stringTemp = toStringHelper(pointer);
        strcat(string, stringTemp);
        pointer = pointer->next;
    }
    return;
}

/*
 * Function: min
 * ----------------------------
 *   Find the min element in linked list
 *
 *   list: struct LinkedList * ptr
 *
 *   return: the mininum struct* Node int the list
 */
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

/*
 * Function: max
 * ----------------------------
 *   Find the max element in linked list
 *
 *   list: struct LinkedList * ptr
 *
 *   return: the maxinium struct* Node int the list
 */
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

/*
 * Function: concat
 * ----------------------------
 *   Concat listTwo after listOne list together.
 *
 *   listOne: pointer to struct LinkedList*  list 1
 *   listTwo: pointer to struct LinkedList*  list 2
 */
void concat(struct LinkedList* listOne, struct LinkedList* listTwo){
    listOne->tail->next = listTwo->head;
    listOne->tail = listTwo->tail;
    listOne->length += listTwo->length;
    return;
}

/*
 * Function: reverse
 * ----------------------------
 *   Reverse the list in-place.
 *
 *   list: pointer to struct LinkedList*  list to be reversed
 */
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

/*
 * Function: getPreviousNodeOfMinHelper
 * ----------------------------
 *   Helper function to get the previous min Node
 *
 *   list: pointer to struct LinkedList*
 *
 *   return: struct Node* previous min node
 */
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

/*
 * Function: getPreviousNodeOfMaxHelper
 * ----------------------------
 *   Helper function to get the previous max Node
 *
 *   list: pointer to struct LinkedList*
 *
 *   return: struct Node* previous max node
 */
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

/*
 * Function: sort
 * ----------------------------
 *   Sort the linked list in place
 *
 *   list: pointer to struct LinkedList*  list to be sorted
 */
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

/*
 * Function: printMenu
 * ----------------------------
 *   print the menu of the linklist application
 */
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
    printf("\tl: Select a value at index\n");
    printf("\tm: Delete linked list\n");
}

/*
 * Function: createList
 * ----------------------------
 *   Initialize and allocate memeory for a struct LinkList*
 *
 *   return: struct LinkedList* ptr of initialized empty link list
 */
struct LinkedList* createList() {
    struct LinkedList* list = malloc(sizeof(struct LinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    return list;
}

/*
 * Function: convertStringToList
 * ----------------------------
 *   Convert char* string to a LinkedList Object
 *   Parses the [1,2,3] str format and return the LinkedList Object
 *
 *   inputStr: string entered by usesr
 *   len: the length of the str
 *
 *   return: LinkedList* object
 */
struct LinkedList* convertStringToList(char *inputStr, int len) {
    // Declare and create a new list to be returned
    struct LinkedList *innerlist = createList();
    
    // remove the starting '[' and ']' in the str
    char * pch;
    inputStr[len-1] = '\0';
    inputStr++;
    
    // get the first value seperated by ','
    pch = strtok (inputStr,",");
    
    // loop through to get all values seperated by ','
    while (pch != NULL)
    {
        // append the object to the innerList
        append(innerlist, *createObjectFromInput(pch));
        pch = strtok (NULL, ",");
    }
    
    // return the completed linkedlist
    return innerlist;
}

/*
 * Function: createObjectFromInput
 * ----------------------------
 *   Convert char* inputValue to a Object
 *   Parses input str format and return the LinkedList Object with correct data type
 *
 *   inputStr: string entered by usesr
 *
 *   return: LinkedList* object
 */
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

/*
 * Function: calculateLength
 * ----------------------------
 *   Calculate how many values are seperated by m
 *
 *   inputStr: string entered by usesr
 *
 *   return: int length of values in inputStr
 */
int calculateLength(char* inputStr) {
    int cnt = 0;
    int strLen = strlen(inputStr);
    for (int i = 0; i<strLen; i++) {
        if (inputStr[i] == ',') {
            cnt++;
        }
    }
    return cnt+1;
}

/*
 * Function: convertStringToIntegerArray
 * ----------------------------
 *   Convert char * input str to a int array
 *
 *   inputStr: string entered by usesr
 *   len: int length of input
 *
 *   return: int array converted from string
 */
int* convertStringToIntegerArray(char *inputStr, int len) {
    int* res = malloc(len * sizeof(int));
    char* ptr = strtok(inputStr, ",");
    int i = 0;
    while (ptr != NULL) {
        res[i++] = atoi(ptr);
        ptr = strtok(NULL, ",");
    }
    return res;
}

/*
 * Function: main
 * ----------------------------
 *   main driver function for linklist
 *
 *   return: status
 */
int main() {
    struct LinkedList *list = createList();
    
    char option;
    char *inputValue = malloc(128 * sizeof(char));
    int index, indicesLength;
    int* indices;
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
                gets(inputValue);
                int indicesLength = calculateLength(inputValue);
                int* indices = convertStringToIntegerArray(inputValue, indicesLength);
                printf("Please input the value you want.\n");
                gets(inputValue);
                if (modify(list, indices, indicesLength, *createObjectFromInput(inputValue)) == FALSE){
                    printf("Invalid index\n");
                }
                free(indices);
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
                printf("Enter a index you want to select in the Linked List.\n");
                gets(inputValue);
                indicesLength = calculateLength(inputValue);
                indices = convertStringToIntegerArray(inputValue, indicesLength);
                printObject(getValue(list, indices, indicesLength));
                printf("\n");
                free(indices);
                break;
            case 'm':
                delete(list);
                list = createList();
                break;
            default:
                return 0;
        }
    }
}

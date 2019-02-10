#include <stdio.h>
#include <stdlib.h>

#define HASHTABLE_SIZE 12

/** Linked Lists **/
typedef struct node{
	int data;
	struct node *next;
}node;

/** #NOTE:
          {node := struct node} **/
/** Hash Table **/
node* get_hashtable(){
    node* hashtable = calloc(HASHTABLE_SIZE, sizeof(struct node)); // calloc for arrays
    return hashtable; // return the actual individual heads
};

/** Inserting Elements  **/
void insert_new_station(node** head, int new_data){
    node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->data = new_data;
    new_node->next = NULL;

    if (*head == NULL){
        *head = new_node;
    } else {
        node* current = *head;

        while(current->next!=NULL){
            current = current->next;
        }
        current->next = new_node;
    }
}

// Hash Function to get a unique index(location)
int get_location(int new_value){
    if(new_value < 0){
        new_value *= -1;
    }

    return new_value % HASHTABLE_SIZE;
}

void insert_into_table(node** hashtable, int new_value){
    int location = get_location(new_value);

    if(hashtable[location] == NULL){
        hashtable[location] = malloc(sizeof(node*));

        node* head = NULL;
        insert_new_station(&head, new_value);

        hashtable[location] = head;
    }else{
        node* head = hashtable[location];
        insert_new_station(&head, new_value);

        hashtable[location] = head;
    }
}

// Print the whole table with the linked lists
void display_table(node** hashtable){
    int i = 0;
    for(i = 0; i < HASHTABLE_SIZE; i++) {
        node* head = hashtable[i];

        printf("%d: ", i);

        if(head == NULL){
            printf("NULL");
        }else{
            node* current = head;

            while(current != NULL){
                printf("%d ", current->data);
                current = current = current->next;
            }
        }
        printf("\n");
    }
}




// Program to create a simple linked list with 3 nodes
int main(int argc, char*argv[]){

    node** table = get_hashtable();

    insert_into_table(table, 1);
    insert_into_table(table, 6);
    insert_into_table(table, 3);
    insert_into_table(table, 2);
    insert_into_table(table, 5);
    insert_into_table(table, 6);

    display_table(table);

//	printf("Hello World of Windows!\n");
//	struct node* head = NULL;
//	struct node* second = NULL;
//	struct node* third = NULL;
//
//	// Allocate 3 nodes in the "heap"
//	head = (struct node*)malloc(sizeof(struct node));
//	second = (struct node*)malloc(sizeof(struct node));
//	third = (struct node*)malloc(sizeof(struct node));
//
//    head->data = 1; // assign data in first node
//    head->next = second; // link first node with second
//
//    second->data = 2;
//    second->next = third;
//
//    third->data = 3;
//    third->next = NULL;
//
//    insert_new_station(head, 4);
//    insert_new_station(head, 5);
//    insert_new_station(head, 6);
//
//    /* Print elements of the linked list */
//    printf("i node: %x\n", third->next->next->next->data);

    // Print the elements of linked list as in array

	return 0;
}

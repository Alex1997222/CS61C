#include "list.h"

/* Add a node to the end of the linked list. Assume head_ptr is non-null. */
void append_node (node** head_ptr, int new_data) {
	/* First lets allocate memory for the new node and initialize its attributes */
	/* YOUR CODE HERE */
	node* newNode = (node*)malloc(sizeof(head_ptr));
	newNode->next = NULL;
	newNode->val = new_data;

	/* If the list is empty, set the new node to be the head and return */
	if (*head_ptr == NULL) {
		*head_ptr = newNode;
		return;
	}

	node* curr = *head_ptr;
	while (curr->next != NULL) {
		curr = curr->next;
	}
	/* Insert node at the end of the list */
	curr->next = newNode;
	return;
}

/* Reverse a linked list in place (in other words, without creating a new list).
   Assume that head_ptr is non-null. */
void reverse_list (node** head_ptr) {
	if(!(*head_ptr) || !(*head_ptr)->next) return;
	node* a = *head_ptr;
	node* b = (*head_ptr)->next;
	while(b){
		node* c = b->next;
		b->next = a;
		a = b,b = c;
	}
	(*head_ptr)->next = NULL;
	*head_ptr = a;
}




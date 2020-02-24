//-----------------------------------------------------------------------------
// List.c
// Implementation file for List ADT, modified for PA2
// John Abendroth
// CruzID: jabendro
// Assignment: PA2
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "List.h"

// Structs -----------------------------------

// Private NodeObj type
typedef struct NodeObj {
	void* data;
	struct NodeObj* next;
	struct NodeObj* prev;
} NodeObj;

// Private Node type
typedef NodeObj* Node;

// Private ListObj type
typedef struct ListObj {
	Node front;
	Node back;
	Node cursor;
	int length;
	int cursor_index;
} ListObj;

// Constructors-Destructors ---------------------------------------------------

// newNode()
// Returns reference to new Node object. Initializes next and data fields
// Private
Node newNode(void* input_data) {
	Node N = malloc(sizeof(NodeObj));
	N->data = input_data;
	N->next = N->prev = NULL;
	return (N);
}

// freeNode()
// Frees heap memory pointed to by *pointerN, sets *pointerN to NULL.
// Private.
void freeNode(Node* pointerN) {
	if (pointerN != NULL && *pointerN != NULL) {
		free(*pointerN);
		*pointerN = NULL;
	}
}


// newList()
// Creates and returns a new empty List
List newList(void) {
	List L;
	L = malloc(sizeof(ListObj));
	L->front = L->back = NULL;
	L->length = 0;
	L->cursor = NULL;
	L->cursor_index = -1;

	return (L);
}

// freeList()
// Frees all heap memory associated with List *pointerL, and sets *pointerL to NULL
void freeList(List* pointerL) {
	if (pointerL != NULL && *pointerL != NULL)
	{
		while ((*pointerL)->length != 0)
		{
			deleteFront(*pointerL);
		}
		free(*pointerL);
		*pointerL = NULL;
	}
}

// Access functions ---------------------------------------------------

// length()
// Returns the number of elements in List L
int length(List L) {
	if (L == NULL) {
		printf("List Error: Calling length() on NULL list reference\n");
		exit(1);
	}
	else
		return (L->length);
}

// index()
// Returns index of cursor element if defined, -1 otherwise
int index(List L) {
	if (L->cursor != NULL) {
		return (L->cursor_index);
	}
	else
		return (-1);
}

// front()
// returns front element of L
// Pre: length() > 0
void* front(List L) {
	if (length(L) == 0)
	{
		printf("List Error: Calling front() on list with length 0\n");
		exit(1);
	}
	else
		return (L->front->data);
}

// back()
// returns back element of List L
// pre: length() > 0
void* back(List L) {
	if (length(L) == 0)
	{
		printf("List Error: Calling back() on list with length 0\n");
		exit(1);
	}
	else
		return (L->back->data);
}

// get()
// Returns cursor element of L
// pre: length() > 0 & index() >= 0
void* get(List L) {
	if (length(L) == 0)
	{
		printf("List Error: Calling get() on list of length 0\n");
		exit(1);
	}
	if (index(L) < 0)
	{
		printf("List Error: Calling get() when cursor is undefined\n");
		exit(1);
	}
	return (L->cursor->data);
}

// Manipulation procedures ----------------------------------------------------

// clear()
// Resets List L to its original empty state
void clear(List L) {

	// Set the cursor to the front, call the delete() function to delete the front cursor,
	// then reset the cursor to the front again until the length is 0 so that moveFront()
	// does nothing, returning index of -1 and exiting the while loop
	moveFront(L);
	while (index(L) >= 0)
	{
		delete(L);
		moveFront(L);
	}
}

// moveFront()
// If L is non-empty, sets cursor to front element, otherwise does nothing
void moveFront(List L) {
	if (L->length > 0)
	{
		L->cursor = L->front;
		L->cursor_index = 0;
	}
}

// moveBack()
// If L is non-empty, sets cursor under the back element, otherwise does nothing
void moveBack(List L) {
	if (L->length > 0)
	{
		L->cursor = L->back;
		L->cursor_index = (L->length - 1);
	}
}

// movePrev()
// If cursor is defined and not at front, move cursor one step toward the front of L; 
// if cursor is defined and at front, cursor becomes undefined; if cursor is undefined do nothing
void movePrev(List L) {
	if (L->cursor != NULL && L->cursor_index != 0)
	{
		L->cursor = L->cursor->prev;
		L->cursor_index--;
	}
	else if (L->cursor_index == 0)
	{
		L->cursor = NULL;
		L->cursor_index = -1;
	}
}

//moveNext()
// If cursor is defined and not at back, move cursor one
// step toward the back of L; if cursor is defined and at
// back, cursor becomes undefined; if cursor is undefined
// do nothing
void moveNext(List L) {
	if (L->cursor != NULL && L->cursor_index != (L->length - 1))
	{
		L->cursor = L->cursor->next;
		L->cursor_index++;
	}
	else if (L->cursor_index == (L->length - 1))
	{
		L->cursor = NULL;
		L->cursor_index = -1;
	}
}

// prepend()
// Insert new element in List L. If L is non-empty, insertion takes place before front element
void prepend(List L, void* data) {
	if (L == NULL) {
		printf("List Error: Calling prepend() on NULL list reference\n");
		exit(1);
	}

	Node new_node = newNode(data);
	if (L->length == 0)
	{
		L->front = L->back = new_node;
	}
	else
	{
		if (L->cursor_index != -1)
			L->cursor_index++;

		Node N = L->front;
		N->prev = new_node;
		new_node->next = N;
		L->front = new_node;
	}

	L->length++;
}

//append()
// Insert new element into List L. If L is non-empty, insertion takes place after back element
void append(List L, void* data) {
	if (L == NULL) {
		printf("List Error: Calling append() on NULL list reference\n");
		exit(1);
	}

	Node node = newNode(data);
	if (L->length == 0)
	{
		L->front = L->back = node;
	}
	else
	{
		L->back->next = node;
		node->prev = L->back;
		L->back = node;
	}

	L->length++;
}

// insertBefore()
// Insert new element before cursor
// Pre: length() > 0, index() >= 0
void insertBefore(List L, void* data) {
	if (L == NULL) {
		printf("List Error: Calling insertBefore() on NULL list reference\n");
		exit(1);
	}
	if (length(L) == 0)
	{
		printf("List Error: Calling insertBefore() on empty List\n");
		exit(1);
	}
	if (index(L) < 0)
	{
		printf("List Error: Calling insertBefore() on List with undefined cursor\n");
		exit(1);
	}

	Node new_node = newNode(data);

	if (L->cursor->prev == NULL || L->cursor == L->front)
	{

		new_node->next = L->cursor;
		L->cursor->prev = new_node;
		L->front = new_node;
	}
	else
	{
		new_node->prev = L->cursor->prev;
		new_node->next = L->cursor;

		L->cursor->prev->next = new_node;
		L->cursor->prev = new_node;
	}

	L->length++;
	L->cursor_index++;
}

// insertAfter()
// Insert new element after cursor
// Pre: length() > 0, index() >= 0
void insertAfter(List L, void* data) {
	if (L == NULL) {
		printf("List Error: Calling insertAfter() on NULL list reference\n");
		exit(1);
	}
	if (length(L) == 0)
	{
		printf("List Error: Calling insertAfter() on empty List\n");
		exit(1);
	}
	if (index(L) < 0)
	{
		printf("List Error: Calling insertAfter() on List with undefined cursor\n");
		exit(1);
	}

	Node new_node = newNode(data);

	if (L->cursor->next == NULL || L->cursor == L->back)
	{
		L->cursor->next = new_node;
		new_node->prev = L->cursor;
		L->back = new_node;
	}
	else
	{
		new_node->prev = L->cursor; //setting the new node's next and prev pointers
		new_node->next = L->cursor->next;

		L->cursor->next->prev = new_node;
		L->cursor->next = new_node;
	}

	L->length++;
}

// deleteFront()
// Delete the front element
// Pre: length() > 0
void deleteFront(List L) {
	if (L == NULL) {
		printf("List Error: Calling deleteFront() on NULL list reference\n");
		exit(1);
	}
	if (length(L) == 0)
	{
		printf("List Error: Calling deleteFront() on empty List\n");
		exit(1);
	}

	if (L->cursor == L->front)
	{
		L->cursor = NULL;
		L->cursor_index = -1;
	}

	Node N = L->front;

	if (L->length > 1)
	{
		L->front = N->next;
		N->next = NULL;
		L->front->prev = NULL;

		L->cursor_index--;
		freeNode(&N);

		//segfault here
		//L->front = L->front->next;
		//L->front->prev = NULL;
	}
	else
	{
		L->front = L->back = NULL;
		freeNode(&N);
	}
	L->length--;
}

// deleteBack()
// Delete the back element
// Pre: length() > 0
void deleteBack(List L) {
	if (L == NULL) {
		printf("List Error: Calling deleteBack() on NULL list reference\n");
		exit(1);
	}
	if (length(L) == 0)
	{
		printf("List Error: Calling deleteBack() on empty List\n");
		exit(1);
	}

	if (L->cursor_index == (L->length - 1))
	{
		L->cursor = NULL;
		L->cursor_index = -1;
	}

	Node N = L->back;
	if (L->length > 1)
	{
		L->back = L->back->prev;
		L->back->next = NULL;
	}
	else
	{
		L->front = L->back = NULL;
	}

	freeNode(&N);
	L->length--;
}

// delete()
// Delete cursor element, making cursor undefined
// Pre: length() > 0, index() >= 0;
void delete(List L) {
	if (L == NULL) {
		printf("List Error: Calling delete() on NULL list reference\n");
		exit(1);
	}
	if (length(L) == 0)
	{
		printf("List Error: Calling delete() on empty List\n");
		exit(1);
	}
	if (index(L) == -1)
	{
		printf("List Error: Calling delete() on List with undefined cursor\n");
		exit(1);
	}

	Node N = L->cursor;
	if (L->length == 1)
	{
		L->front = L->back = L->cursor = NULL;
		L->cursor_index = -1;
	}
	else
	{
		if (L->cursor_index == 0)
		{
			L->front = L->front->next;
			L->front->prev = NULL;

			L->cursor = NULL;
			L->cursor_index = -1;
		}
		else if (L->cursor_index == (L->length - 1))
		{
			L->back = L->back->prev;
			L->back->next = NULL;

			L->cursor = NULL;
			L->cursor_index = -1;
		}
		else
		{
			N->prev->next = N->next;
			N->next->prev = N->prev;

			L->cursor = NULL;
			L->cursor_index = -1;
		}
	}

	freeNode(&N);
	L->length--;
}

// not required anymore, may not work

// printList()
// Prints to the file pointed to by out, a string representation of L consisting of a space separated sequence of integers, with front on left
void printList(FILE* out, List L) {
	if (L == NULL) {
		printf("List Error: Calling printList() on NULL list reference\n");
		exit(1);
	}

	moveFront(L);
	while (index(L) >= 0)
	{
        int x = (int)get(L);
		fprintf(out, "%d ", x);
		moveNext(L);
	}

}


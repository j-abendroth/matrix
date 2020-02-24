#pragma once

//-----------------------------------------------------------------------------
// List.h
// Header file for List ADT
// John Abendroth
// CruzID: jabendro
// Assignment: PA2
//-----------------------------------------------------------------------------

#ifndef _LIST_H_INCLUDE_
#define _LIST_H_INCLUDE_

// Exported type
typedef struct ListObj* List;

//Constructors & Destructors -------------------------------------

// newList()
// Creates and returns a new empty List
List newList(void);

// freeList(List *pL);
// Frees all memory associated with *pL and sets *pL to null
void freeList(List* pL);


// Access functions ----------------------------------------

// length()
// Returns the number of elements in the List L
int length(List L);

// index()
// Returns the index of cursor element if defined, or -1 otherwise
int index(List L);

// front()
// Returns front element of List L
// Pre: length() > 0
void* front(List L);

// back()
// Returns back element of List L
// Pre: length() > 0
void* back(List L);

// get()
// Returns cursor element of List L
// Pre: length() > 0, index() >= 0
void* get(List L);

// Manipulation procedures ----------------------------------------------------

// clear()
// Resets List L to its original empty state
void clear(List l);

// moveFront()
// If L is non-empty, sets cursor under the front element, otherwise does nothing
void moveFront(List L);

// movePrev()
// If cursor is defined and not at front, move cursor one step toward the front of L. if cursor is defined and at front, cursor beceomes undefined; if cursor is undefined do nothing
void movePrev(List L);

//moveBack()
//If L is non-empty, sets cursor under the back element, otherwise does nothing
void moveBack(List L);

// moveNext()
// If cursor is defined and not at back, move cursor one
 // step toward the back of L; if cursor is defined and at
 // back, cursor becomes undefined; if cursor is undefined
 // do nothing
void moveNext(List L);

// prepend()
// Insert new element void*o L. If L is non-empty, insertion takes place before front element
void prepend(List L, void* data);

// append()
// Insert new element void*o L. If L is non-empty insertion takes place after back element
void append(List l, void* data);

// insertBefore()
// insert new element before cursor
// Pre: length() > 0, index() > 0
void insertBefore(List L, void* data);

// insertAfter()
// Insert new element before cursor 
// Pre: length() > 0, index() >= 0
void insertAfter(List L, void* data);

// deleteFront()
// Delete the front element
// Pre: length() > 0
void deleteFront(List L);

// deleteBack()
// Delete the back element
// Pre: length() > 0
void deleteBack(List L);

// delete()
// Delete cursor element, making cursor undefined
// Pre: length() > 0, index() >= 0
void delete(List L);

// Other operations -----------------------------------------------------------

//not required anymore, may not work

// printList()
// Prints to the file pointed to by out, a string representation of L consisting of a space separated sequence of integers, with front on left
void printList(FILE* out, List L);

#endif

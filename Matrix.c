//-----------------------------------------------------------------------------
// Matrix.c
// Implementation file for Matrix ADT
// John Abendroth
// CruzID: jabendro
// Assignment: PA2
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "List.h"
#include "Matrix.h"

// Structs -----------------------------------

// Private EntryObj type
typedef struct EntryObj {
	int column;
	double value;
} EntryObj;

// Private Node type
typedef EntryObj* Entry;

typedef struct MatrixObj {
	int NNZ;
	int size;
	List* row;
}MatrixObj;

// Constructors-Destructors ---------------------------------------------------

//newEntry()
//Creates new entry and inputs the column position and value
Entry newEntry(int col, double val) {
	Entry E = malloc(sizeof(EntryObj));
	E->column = col;
	E->value = val;

	return (E);
}

// newMatrix()
// Returns a reference to a new nXn Matrix object in the zero state
Matrix newMatrix(int n) {
    Matrix M;
    M = malloc(sizeof(MatrixObj));
    M->row = malloc((n+1) * (sizeof(List)));
	M->NNZ = 0;
	M->size = n;

    for (int i = 1; i <= n; i++)
	{
        M->row[i] = newList();
	}
	return(M);
}

// freeEntry()
// Frees heap memory associated with *pE, sets *pE to NULL
void freeEntry(Entry* pE) {
	if (pE != NULL && *pE != NULL) {
		free(*pE);
		*pE = NULL;
	}
}

// freeMatrix()
// Frees heap memory associated with *pM, sets *pM to NULL.
void freeMatrix(Matrix* pM) {
	if (pM != NULL && *pM != NULL) {
		for (int i = 1; i <= (*pM)->size ; i++)
		{
            if ((length(((*pM)->row[i]))) != 0)
            {
                moveFront(((*pM)->row[i]));
                while (index(((*pM)->row[i])) >= 0)
                {
                    Entry E = (Entry)(get(((*pM)->row[i])));
                    freeEntry(&E);

                    moveNext(((*pM)->row[i]));
                }
            }
			freeList(&((*pM)->row[i]));
		}
        free((*pM)->row);
		free(*pM);
		*pM = NULL;
	}
}

// Access functions ---------------------------------------------------

// size()
// Return the size of square Matrix M
int size(Matrix M) {
	if (M == NULL) {
		printf("Matrix Error: Calling size() on NULL matrix reference\n");
		exit(1);
	}
	return (M->size);
}

// NNZ()
// Return the number of non-zero elements in M
int NNZ(Matrix M) {
	if (M == NULL) {
		printf("Matrix Error: Calling NNZ() on NULL matrix reference\n");
		exit(1);
	}
	return (M->NNZ);
}

// equals()
// Return true (1) if matrices A and B are equal, false (0) otherwise.
int equals(Matrix A, Matrix B) {
	if (A == NULL || B == NULL) {
		printf("Matrix Error: Calling equals() on NULL matrix reference\n");
		exit(1);
	}
	if (size(A) != size(B))
	{
		return (0);
	}
    if (A == B)
    {
        return (1);
    }

	for (int i = 1; i <= size(A); i++)
	{
        if (length(A->row[i]) != length(B->row[i]))
        {
            return (0);
        }
        
		moveFront(A->row[i]);
        moveFront(B->row[i]);
		while (index(A->row[i]) >= 0 && index(B->row[i]) >= 0)
        {
            Entry EA = (Entry)(get(A->row[i]));
            Entry EB = (Entry)(get(B->row[i]));
            
            if (EA->value != EB->value)
            {
                return (0);
            }
            
            moveNext(A->row[i]);
            moveNext(B->row[i]);
        }
    }
    
    return(1);
}

// Manipulation procedures ----------------------------------------------------

// makeZero()
// Re-sets M to the zero Matrix.
void makeZero(Matrix M) {
	if (M == NULL) {
		printf("Matrix Error: Calling makeZero() on NULL matrix reference\n");
		exit(1);
	}
    
	for (int i = 1; i <= size(M); i++)
	{
        moveFront(M->row[i]);
        while (index(M->row[i]) >= 0)
        {
            Entry E = (Entry)(get(M->row[i]));
            freeEntry(&E);
            

            moveNext(M->row[i]);
        }
        
		clear(M->row[i]);
	}
	M->NNZ = 0;
}

// changeEntry()
// Changes the ith row, jth column of M to the value x.
// Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x) {
	if (M == NULL) {
		printf("Matrix Error: Calling changeEntry() on NULL matrix reference\n");
		exit(1);
	}
	if (1 > i || i > size(M)) {
		printf("Matrix Error: Calling changeEntry() with invalid i row size\n");
		exit(1);
	}
	if (1 > j || j > size(M)) {
		printf("Matrix Error: Calling changeEntry() with invalid j column size\n");
		exit(1);
	}
	bool j_found = false;

    Entry E;
    
	moveFront(M->row[i]);
	while (index(M->row[i]) >= 0)
	{
		E = (Entry)(get(M->row[i]));
		if (E->column < j)
		{
			moveNext(M->row[i]);
		}
		else if (E->column == j)
		{
			j_found = true;
            break;
		}
		else
		{
			// E->column > j, break
			// cursor should stay defined as the next list element not smaller than j or equal to j
			break;
		}
	}

	if (!j_found && x == 0.0) //case 1, Mij is 0 and x is 0: do nothing
	{
		// do nothing, value isn't in list and don't insert 0s
	}
	else if (j_found && x == 0.0) //case 2, Mij is not 0 and x is 0: delete current entry at Mij
	{
		freeEntry(&E);
		delete(M->row[i]);
		M->NNZ--;
	}
	else if (!j_found && x != 0.0) //case 3, Mij is 0 and x is not 0: insert new entry
	{
		Entry new_entry = newEntry(j, x);

		if (length(M->row[i]) == 0 || index(M->row[i]) == -1)
		{
			append(M->row[i], new_entry);
		}
		else
		{
			insertBefore(M->row[i], new_entry);
		}

		M->NNZ++;
	}
	else //case 4, Mij is not 0, x is not 0: overwrite existing value
	{
	E->value = x;
	}
}

// Matrix Arithmetic operations ----------------------------------------------------

int ret_col(Entry E){
    return(E->column);
}

double ret_value(Entry E){
    return(E->value);
}

// copy()
// Returns a reference to a new Matrix object having the same entries as A.
Matrix copy(Matrix A) {
	if (A == NULL) {
		printf("Matrix Error: Calling copy() on NULL matrix reference\n");
		exit(1);
	}

	Matrix B = newMatrix(size(A));

	for (int i = 1; i <= size(A); i++)
	{
		moveFront(A->row[i]);
		while (index(A->row[i]) >= 0)
		{
			Entry E = (Entry)((get(A->row[i])));
            int col = ret_col(E);
            double val = ret_value(E);
            Entry new_E = newEntry(col, val);
			append(B->row[i], new_E);

			moveNext(A->row[i]);
		}
	}
	B->size = A->size;
	B->NNZ = A->NNZ;

	return (B);
}

// transpose()
// Returns a reference to a new Matrix object representing the transpose
// of A.
Matrix transpose(Matrix A) {
	if (A == NULL) {
		printf("Matrix Error: Calling transpose() on NULL matrix reference\n");
		exit(1);
	}

	Matrix B = newMatrix(size(A));

	for (int i = 1; i <= size(A); i++)
	{
		if (length(A->row[i]) > 0)
		{
			moveFront(A->row[i]);
			while (index(A->row[i]) >= 0)
			{
				Entry E = (Entry)(get(A->row[i]));
                int col = ret_col(E);
                double val = ret_value(E);
				changeEntry(B, col, i, val);
                
                moveNext(A->row[i]);
			}
		}
	}
	B->size = A->size;
	return(B);
}

// scalarMult()
// Returns a reference to a new Matrix object representing xA.
Matrix scalarMult(double x, Matrix A) {
	if (A == NULL) {
		printf("Matrix Error: Calling scalarMult() on NULL matrix reference\n");
		exit(1);
	}

	Matrix M = newMatrix(size(A));

	for (int i = 1; i <= size(A); i++)
	{
		if (length(A->row[i]) == 0) continue;

		//possibly switch the append() to a changeentry()
		moveFront((A->row[i]));
		while (index((A->row[i])) >= 0)
		{
			Entry E = (Entry)(get((A->row[i])));
            int col = ret_col(E);
			double new_value = E->value * x;
			Entry new_entry = newEntry(col, new_value);
			append((M->row[i]), new_entry);

			moveNext((A->row[i]));
		}
	}

	M->size = A->size;
	M->NNZ = A->NNZ;
	return(M);
}

// rowSum()
// Computes the sum of 2 matrix rows repsented by lists P and Q and returns a new list as the result
List rowSum(List P, List Q) {
	if (P == NULL || Q == NULL) {
		printf("Matrix Error: Calling rowSum() on a NULL list reference\n");
		exit(1);
	}
	if (length(P) == 0) {
		return(Q);
	}
	if (length(Q) == 0) {
		return(P);
	}

	List new_row = newList();

	moveFront(P);
	moveFront(Q);
	while (index(P) >= 0 || index(Q) >= 0)
	{
		double sum = 0.0;

		if (index(P) >= 0 && index(Q) >= 0)
		{
            Entry rowp = (Entry)(get(P));
            Entry rowq = (Entry)(get(Q));
            
			if (rowp->column < rowq->column) //means there's a zero entry in rowq at rowp col, append new list with rowp value
			{
				Entry new_entry = newEntry((rowp->column), (rowp->value));
				append(new_row, new_entry);

				moveNext(P);
			}
			else if (rowp->column > rowq->column) //means there's a zero entry in rowp at rowq col, append new list with rowq value
			{
				Entry new_entry = newEntry((rowq->column), (rowq->value));
				append(new_row, new_entry);

				moveNext(Q);
			}
			else //rowp col == rowq col
			{
				sum = rowp->value + rowq->value;
                if(sum != 0)
                {
                    Entry new_entry = newEntry((rowp->column), sum);
                    append(new_row, new_entry);
                }

				moveNext(P);
				moveNext(Q);
			}
		}
		else if (index(P) >= 0) //rest of entries in row q are 0s, just append row p entries until there are no more
		{
            Entry rowp = (Entry)(get(P));
            
			Entry new_entry = newEntry((rowp->column), (rowp->value));
			append(new_row, new_entry);

			moveNext(P);
		}
		else //Only Q index isn't -1, just append row q entries until there are no more
		{
            Entry rowq = (Entry)(get(Q));
            
			Entry new_entry = newEntry((rowq->column), (rowq->value));
			append(new_row, new_entry);

			moveNext(Q);
		}
	}

	return(new_row);
}

// sum()
// Returns a reference to a new Matrix object representing A+B.
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B) {
	if (A == NULL || B == NULL) {
		printf("Matrix Error: Calling sum() on NULL matrix reference\n");
		exit(1);
	}
	if (size(A) != size(B)) {
		printf("Matrix Error: Calling sum() on two matrices whose sizes aren't equal\n");
		exit(1);
	}
    if (equals(A,B) == 1)
    {
        Matrix M = scalarMult(2, A);
        return (M);
    }
	if (NNZ(A) == 0)
	{
		return(B);
	}
	if (NNZ(B) == 0)
	{
		return(A);
	}
    

	Matrix M = newMatrix(size(A));

	for (int i = 1; i <= size(A); i++)
	{
        freeList(&(M->row[i]));
		M->row[i] = rowSum((A->row[i]), (B->row[i]));
        M->NNZ += (length(M->row[i]));
	}

    M->size = A->size;
	return(M);
}

// rowSub()
// Computes the difference of 2 matrix rows repsented by lists P and Q and returns a new list as the result of row P - row Q
List rowSub(List P, List Q) {
	if (P == NULL || Q == NULL) {
		printf("Matrix Error: Calling rowSum() on a NULL list reference\n");
		exit(1);
	}
	if (length(Q) == 0) {
		return(P);
	}

	List new_row = newList();

	if (length(P) == 0) {
		moveFront(Q);
		while (index(Q) >= 0)
		{
			Entry rowq = (Entry)(get(Q));
			double diff = rowq->value * (-1);
			Entry new_entry = newEntry((rowq->column), diff);
			append(new_row, new_entry);

			moveNext(Q);
		}
		return(new_row);
	}

	moveFront(P);
	moveFront(Q);
	while (index(P) >= 0 || index(Q) >= 0)
	{
		double diff = 0.0;

		if (index(P) >= 0 && index(Q) >= 0)
		{
            Entry rowp = (Entry)(get(P));
            Entry rowq = (Entry)(get(Q));
			if (rowp->column < rowq->column) //means there's a zero entry in rowq at rowp col, append new list with rowp value
			{
				Entry new_entry = newEntry((rowp->column), (rowp->value));
				append(new_row, new_entry);

				moveNext(P);
			}
			else if (rowp->column > rowq->column) //means there's a zero entry in rowp at rowq col, append new list with -rowq value
			{
				diff = rowq->value * (-1);
				Entry new_entry = newEntry((rowq->column), diff);
				append(new_row, new_entry);

				moveNext(Q);
			}
			else //rowp col == rowq col
			{
				diff = rowp->value - rowq->value;
                if (diff != 0)
                {
                    Entry new_entry = newEntry((rowp->column), diff);
                    append(new_row, new_entry);
                }

				moveNext(P);
				moveNext(Q);
			}
		}
		else if (index(P) >= 0) //rest of entries in row q are 0s, just append row p entries until there are no more
		{
            Entry rowp = (Entry)(get(P));
			Entry new_entry = newEntry((rowp->column), (rowp->value));
			append(new_row, new_entry);

			moveNext(P);
		}
		else //Only Q index isn't -1, just append row -q entries until there are no more
		{
            Entry rowq = (Entry)(get(Q));
			diff = rowq->value * (-1);
			Entry new_entry = newEntry((rowq->column), diff);
			append(new_row, new_entry);

			moveNext(Q);
		}
	}
	return(new_row);
}

// diff()
// Returns a reference to a new Matrix object representing A-B.
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B) {
	if (A == NULL || B == NULL) {
		printf("Matrix Error: Calling diff() on NULL matrix reference\n");
		exit(1);
	}
	if (size(A) != size(B)) {
		printf("Matrix Error: Calling diff() on two matrices whose sizes aren't equal\n");
		exit(1);
	}
    if (equals(A, B) == 1)
    {
        Matrix M = newMatrix(size(A));
        return (M);
    }
	if (NNZ(B) == 0)
	{
		return(A);
	}
	if (NNZ(A) == 0)
	{
		Matrix sub = scalarMult(-1.0, B);
		return(sub);
	}

	Matrix M = newMatrix(size(A));

	for (int i = 1; i <= size(A); i++)
	{
        freeList(&(M->row[i]));
		M->row[i] = rowSub((A->row[i]), (B->row[i]));
        M->NNZ += (length(M->row[i]));
	}

	return(M);
}

// vectorDot()
// Computes the vector dot product of two matrix rows represented by lists P and Q
double vectorDot(List P, List Q) {
	if (P == NULL || Q == NULL) {
		printf("Matrix Error: Calling vectorDor() on a NULL list reference\n");
		exit(1);
	}
	if (length(P) == 0 || length(Q) == 0) {
		return(0.0);
	}
	double sum = 0.0;

	moveFront(P);
	moveFront(Q);
	while (index(P) >= 0 && index(Q) >= 0)
	{
		Entry row1 = (Entry)(get(P));
		Entry row2 = (Entry)(get(Q));

		if (row1->column < row2->column)
		{
			moveNext(P);
		}
		else if (row1->column > row2->column)
		{
			moveNext(Q);
		}
		else
		{
			sum += ((row1->value) * (row2->value));
			moveNext(P);
			moveNext(Q);
		}
	}
	return(sum);
}

// product()
// Returns a reference to a new Matrix object representing AB
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B) {
	if (A == NULL || B == NULL) {
		printf("Matrix Error: Calling product() on NULL matrix reference\n");
		exit(1);
	}
	if (size(A) != size(B)) {
		printf("Matrix Error: Calling product() on two matrices whose sizes aren't equal\n");
		exit(1);
	}

	Matrix M = newMatrix(size(A));

	if (NNZ(A) == 0 || NNZ(B) == 0)
	{
		return(M);
	}

	Matrix B_t = transpose(B);

	for (int i = 1; i <= size(A); i++)
	{
		if (length(A->row[i]) == 0) continue;

		for (int j = 1; j <= size(B); j++)
		{
			if (length(B_t->row[j]) == 0) continue;
            
			changeEntry(M, i, j, (vectorDot(A->row[i], B_t->row[j])));
		}
	}

	freeMatrix(&B_t);

	return(M);
}

// printMatrix()
// Prints a string representation of Matrix M to filestream out. Zero rows
// are not printed. Each non-zero is represented as one line consisting
// of the row number, followed by a colon, a space, then a space separated
// list of pairs "(col, val)" giving the column numbers and non-zero values
// in that row. The double val will be rounded to 1 decimal point.
void printMatrix(FILE* out, Matrix M) {
	if (M == NULL) {
		printf("Matrix Error: Calling printMatrix() on NULL matrix reference\n");
		exit(1);
	}

	for (int i = 1; i <= size(M); i++)
	{
        if (length(M->row[i]) != 0)
        {
            fprintf(out, "%d: ", i);

            moveFront(M->row[i]);
            while (index(M->row[i]) >= 0)
            {
                Entry E = (Entry)(get(M->row[i]));
                fprintf(out, "(%d, %0.1f) ", E->column, E->value);

                moveNext(M->row[i]);
            }
            fprintf(out, "\n");
        }
	}
}

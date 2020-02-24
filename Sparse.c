//-----------------------------------------------------------------------------
// Sparce.c
// Implementation file for Matrix client module
// John Abendroth
// CruzID: jabendro
// Assignment: PA2
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Matrix.h"

int main(int argc, char * argv[]){
    int i, j;
    double val;
    FILE *in, *out;

   // check command line for correct number of arguments
   if( argc != 3 ){
       fprintf(stderr, "Incorrect argument format!");
      exit(1);
   }

   // open files for reading and writing
   in = fopen(argv[1], "r");
   out = fopen(argv[2], "w");
   if( in==NULL ){
      printf("Unable to open file %s for reading\n", argv[1]);
      exit(1);
   }
   if( out==NULL ){
      printf("Unable to open file %s for writing\n", argv[2]);
      exit(1);
   }
    
    fscanf(in, "%d", &i);
    Matrix A = newMatrix(i);
    Matrix B = newMatrix(i);
    
    fscanf(in, "%d", &i);
    int a = i;
    fscanf(in, "%d", &i);
    int b = i;
    
    while(NNZ(A) < a)
    {
        fscanf(in, "%d", &i);
        fscanf(in, "%d", &j);
        fscanf(in, "%lf", &val);
        changeEntry(A, i, j, val);
    }
    
    while(NNZ(B) < b)
    {
        fscanf(in, "%d", &i);
        fscanf(in, "%d", &j);
        fscanf(in, "%lf", &val);
        changeEntry(B, i, j, val);
    }
    
    fprintf(out, "A has %d non-zero entries:\n", NNZ(A));
    printMatrix(out, A);
    
    fprintf(out, "\n");
    
    fprintf(out, "B has %d non-zero entries:\n", NNZ(B));
    printMatrix(out, B);
    
    fprintf(out, "\n");
    
    Matrix C = scalarMult(1.5, A);
    fprintf(out, "(1.5)*A =\n");
    printMatrix(out, C);
    
    fprintf(out, "\n");
    
    Matrix D = sum(A, B);
    fprintf(out, "A+B =\n");
    printMatrix(out, D);
    
    fprintf(out, "\n");
    
    Matrix E = sum(A, A);
    fprintf(out, "A+A =\n");
    printMatrix(out, E);
    
    fprintf(out, "\n");
    
    Matrix F = diff(B, A);
    fprintf(out, "B-A =\n");
    printMatrix(out, F);
    
    fprintf(out, "\n");
    
    Matrix G = diff(A, A);
    fprintf(out, "A-A =\n");
    printMatrix(out, G);
    
    fprintf(out, "\n");
    
    Matrix H = transpose(A);
    fprintf(out, "Transpose(A) =\n");
    printMatrix(out, H);
    
    fprintf(out, "\n");
    
    Matrix I = product(A, B);
    fprintf(out, "A*B =\n");
    printMatrix(out, I);
    
    fprintf(out, "\n");
    
    Matrix J = product(B, B);
    fprintf(out, "B*B =\n");
    printMatrix(out, J);
    
    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&C);
    freeMatrix(&D);
    freeMatrix(&E);
    freeMatrix(&F);
    freeMatrix(&G);
    freeMatrix(&H);
    freeMatrix(&I);
    freeMatrix(&J);

   /* close files */
    fclose(in);
    fclose(out);
    
   return(0);
}

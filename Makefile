# Makefile for CSE 101 Programming Assignment 2
# CruzID: jabendro
#
#       make                   makes Sparse
#       make MatrixClient      makes MatrixClient
#       make ListClient        makes ListClient
#       make MatrixTest        makes MatrixTest
#       make ListTest          makes ListTest
#       make memcheck          makes memcheck (valgrind)
#------------------------------------------------------------------------------

Sparse: Sparse.o Matrix.o  List.o
	gcc -o Sparse Sparse.o Matrix.o List.o

Sparse.o : Sparse.c Matrix.h
	gcc -c -std=c99 -Wall Sparse.c

Matrix.o : Matrix.c Matrix.h List.h
	gcc -c -std=c99 -Wall Matrix.c

List.o : List.c List.h
	gcc -c -std=c99 -Wall List.c

ListClient : ListClient.o List.o
	gcc -o ListClient ListClient.o List.o

ListClient.o : ListClient.c List.h
	gcc -c -std=c99 -Wall ListClient.c

MatrixClient : MatrixClient.o Matrix.o List.o
	gcc -o MatrixClient MatrixClient.o Matrix.o List.o

MatrixClient.o : MatrixClient.c Matrix.h List.h
	gcc -c -std=c99 -Wall MatrixClient.c

MatrixTest : MatrixTest.o Matrix.o List.o
	gcc -o MatrixTest MatrixTest.o Matrix.o List.o

MatrixTest.o : MatrixTest.c Matrix.h List.h
	gcc -c -std=c99 -Wall MatrixTest.c

ListTest : ListTest.o List.o
	gcc -o ListTest ListTest.o List.o

ListTest.o : ListTest.c List.h
	gcc -c -std=c99 -Wall ListTest.c

memcheck : Sparse 
	valgrind --leak-check=full -v Sparse in.txt out.txt

clean :
	rm -f Sparse MatrixClient MatrixTest ListTest *.o *.txt
/* FileName     : param.cxx
 * Author       : Zachary Zuch
 * Course       : CSCI 330-01
 * Assignment   : Jensen's Device
 * Description  : This program has the user setup two vectors and two
 *                matrices, the vectors will produce the norm and dot
 *                product of the vector, and the matrices will just
 *                multiplied together.
************************************************************************/
// Includes and Macro

#include <iostream>
#include <cstdio>
#include <math.h>
#include "tvector.h"
#include "tmatrix.h"

#define jensen(expr, index, start, stop, out) {\
   double temp = 0;\
   for (index = start; index < stop; ++index) {\
      temp += (expr);\
   }\
   out = temp;\
}

/***********************************************************************/
// Prototypes

void 
read(tvector<double> *vec);

void 
read(tmatrix<double> *matr);

void 
write(tvector<double> vec);

void 
write(tmatrix<double> matr);

double
norm(tvector<double> vec);

double 
multiply(tvector<double> vec, tvector<double> vec2);

tmatrix<double>*
multiply(tmatrix<double> matr, tmatrix<double> matr2);

/***********************************************************************/
// Functions

int 
main( )
{
   // Take input for the size of the vectors and matrices
   std::cout << std::endl;
   std::cout << "INTEGERS J, K, and L" << std::endl;
   int j;
   std::cout << "J ==> ";
   std::cin >> j;
   
   int k;
   std::cout << "K ==> ";
   std::cin >> k;

   int l;
   std::cout << "L ==> ";
   std::cin >> l;
   std::cout << std::endl;

   // Create Vector and Matrix objects and fill them in with
   // standard input
   tvector<double> vec(j);
   std::cout << "----- VECTOR A -----" << std::endl;
   read(&vec);

   tvector<double> vec2(j);
   std::cout << "----- VECTOR B -----" << std::endl;
   read(&vec2);

   tmatrix<double> matrJK(j, k);
   std::cout << "--- MATRIX J x K ---" << std::endl;
   read(&matrJK);

   tmatrix<double> matrKL(k, l);
   std::cout << "--- MATRIX K x L ---" << std::endl;
   read(&matrKL);

   // Print out the vectors, matrices, and the values from the
   // operations performed on the vectors and matrices
   std::cout << "a is ";
   write(vec);
   printf("Norm of a is %.2f\n", norm(vec));
   std::cout << "b is ";
   write(vec2);
   printf("Norm of b is %.2f\n", norm(vec2));
   printf("Dot product of a and b is %.2f\n", multiply(vec, vec2));
   std::cout << "c is " << std::endl;
   write(matrJK);
   std::cout << "d is " << std::endl;
   write(matrKL);
   std::cout << "The product of c and d is " << std::endl;
   write((*multiply(matrJK, matrKL)));

   return 0;
}

/***********************************************************************/
// Enters values into vector from standard input
void 
read(tvector<double> *vec)
{
   int size = (*vec).size();
   int i;
   for (i = 0; i < size; ++i)
   {
      std::cout << "VECTOR [" << i << "] ==> ";
      std::cin >> (*vec)[i];
   }
   std::cout << std::endl;
}

/***********************************************************************/
// Enters values into matrix in row-major order from standard input
void 
read(tmatrix<double> *matr)
{
   int rows = (*matr).numrows();
   int cols = (*matr).numcols();
   int i;
   int j;
   for (i = 0; i < rows; ++i)
   {
      for (j = 0; j < cols; ++j)
      {
         std::cout << "MATRIX [" << i << "] [" << j << "] ==> ";
         std::cin >> (*matr)[i][j];
      }
   }
   std::cout << std::endl;
}

/***********************************************************************/
// Prints out a vector with the designated format
void 
write(tvector<double> vec)
{
   int size = vec.size();
   int i;
   for (i = 0; i < size; ++i)
   {
      printf("%8.2f", vec[i]);
   }
   std::cout << std::endl;
}

/***********************************************************************/
// Prints out a matrix with the designated format
void 
write(tmatrix<double> matr)
{
   int rows = matr.numrows();
   int cols = matr.numcols();
   int i;
   int j;
   for (i = 0; i < rows; ++i)
   {
      for (j = 0; j < cols; ++j)
      {
         printf("%8.2f", matr[i][j]);
      }
      std::cout << std::endl;
   }
   std::cout << std::endl;
}

/***********************************************************************/
// Returns the norm of a vector 
double
norm(tvector<double> vec)
{
   double sum;
   int size = vec.size();
   int i;
   jensen(vec[i] * vec[i], i, 0, size, sum)
   return sqrt(sum);
}

/***********************************************************************/
// Returns the dot product of two vectors
double 
multiply(tvector<double> vec, tvector<double> vec2)
{
   double sum;
   int size = vec.size();
   int i;
   jensen(vec[i] * vec2[i], i, 0, size, sum)
   return sum;
}

/***********************************************************************/
// Multiplies two matrices together and returns a new matrix
tmatrix<double>* 
multiply(tmatrix<double> matr, tmatrix<double> matr2)
{
   int matrRows = matr.numrows();
   int matrCols = matr.numcols();
   int matr2Cols = matr2.numcols();
   tmatrix<double> *newMatrix = new tmatrix<double>(matrRows, matr2Cols);
   int i;
   int j;
   int k;
   for (i = 0; i < matrRows; ++i)
   {
      for (j = 0; j < matr2Cols; ++j)
      {
         jensen(matr[i][k] * matr2[k][j], k, 0, matrCols, (*newMatrix)[i][j])
      }
   }
   return newMatrix;
}

/***********************************************************************/
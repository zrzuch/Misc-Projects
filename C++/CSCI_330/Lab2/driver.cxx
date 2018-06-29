#include <iostream>
#include "lbvector.h"

using namespace std;

int main ( )
{
   cout << "creating a single element vector with index 42" << endl;

   lbvector<int> fortyTwo(42,42);
   fortyTwo[42] = 24;
   if (fortyTwo.capacity( ) != 1) {
      cout << "Failed construction capacity. capacity( ) should return 1." << endl;
   }
   if (fortyTwo.lower( ) != 42) {
      cout << "Failed construction lower. lower( ) should return 42." << endl;
   }
   if (fortyTwo.upper( ) != 42) {
      cout << "Failed construction upper. upper( ) should return 42." << endl;
   }
   cout << "checking initialization" << endl;
   if (fortyTwo[42] != 24) {
      cout << "Failed because fortyTwo[42] should equal 24." << endl;
   }

   cout << "creating vector of int" << endl;
   const int vLow = -3;
   const int vUp = 3;
   const int vInit = 17;

   lbvector<int> v (vLow, vUp, vInit);
   cout << "vector created" << endl;
   int i;

   if (v.lower( ) != vLow) {
      cout << "Failed construction lower." << endl;
   }
   if (v.upper( ) != vUp) {
      cout << "Failed construction upper." << endl;
   }
   cout << "checking initialization" << endl;
   for (i = vLow; i <= vUp; i++) {
      if (v[i] != vInit) {
         cout << "Failed initialization test of integer vector." << endl;
      }
   }

   cout << "creating vector of char" << endl;
   const int cvLow = 4;
   const int cvUp = 8;
   const char cvInit = '?';
   lbvector<char> cv (cvLow, cvUp, cvInit);
   cout << "vector created" << endl;
   int j;

   if (cv.lower( ) != cvLow) {
      cout << "Failed construction lower." << endl;
   }
   if (cv.upper( ) != cvUp) {
      cout << "Failed construction upper." << endl;
   }
   cout << "checking initialization" << endl;
   for (j = cvLow; j <= cvUp; j++) {
      if (cv[j] != cvInit) {
         cout << "Failed initialization test of char vector." << endl;
      }
   }

   cout << "creating vector of double" << endl;
   const int dvLow = -17;
   const int dvUp = -8;
   const double dvInit = 3.141592653589793;
   lbvector<double> dv (dvLow, dvUp, dvInit);
   cout << "vector created" << endl;
   int k;

   if (dv.lower( ) != dvLow) {
      cout << "Failed construction lower." << endl;
   }
   if (dv.upper( ) != dvUp) {
      cout << "Failed construction upper." << endl;
   }
   cout << "checking initialization" << endl;
   for (k = dvLow; k <= dvUp; k++) {
      if (dv[k] != dvInit) {
         cout << "Failed initialization test of double vector." << endl;
      }
   }

   cout << "checking assignment" << endl;
   for (i = vLow; i <= vUp; i++) {
      v[i] = i;
   }
   for (i = vLow; i <= vUp; i++) {
      if (v[i] != i) {
         cout << "Failed assignment test." << endl;
      }
   }
   cout << "Assignment test complete" << endl;

   cout << "checking resize raising upper and lower" << endl;
   const int vLow2 = vLow+1;
   const int vUp2 = vUp+1;
   v.resize(vLow2, vUp2);
   if (v.lower( ) != vLow2) {
      cout << "Failed resize lower" << endl;
   }
   if (v.upper( ) != vUp2) {
      cout << "Failed resize upper" << endl;
   }
   cout << "resize complete" << endl;
   cout << "checking resize copying" << endl;
   v[vUp2] = vUp2;
   for (i=vLow2; i<=vUp2; i++) {
      if (v[i] != i) {
         cout << "Failed resize copy test." << endl;
      }
   }
   cout << "resize copy test complete" << endl;

   cout << "checking resize lowering lower and upper" << endl;
   const int vLow3 = vLow-1;
   const int vUp3 = vUp-1;  
   v.resize(vLow3, vUp3);
   if (v.lower( ) != vLow3) {
      cout << "Failed resize lower" << endl;
   }
   if (v.upper( ) != vUp3) {
      cout << "Failed resize upper" << endl;
   }
   cout << "resize complete" << endl;
   cout << "checking resize copying" << endl;
   v[vLow3] = vLow3;
   v[vLow3+1] = vLow3+1;
   for (i=vLow3; i<=vUp3; i++) {
      if (v[i] != i) {
         cout << "Failed resize copy test." << endl;
      }
   }
   cout << "resize copy test complete" << endl;

   cout << "constructing an empty vector" << endl;
   lbvector<double> e;
   if (e.capacity( ) != 0) {
      cout << "size of empty vector should be zero" << endl;
   }
   cout << "resizing the empty vector" << endl;
   e.resize(-1, 3);
   
   if (e.lower( ) != -1) {
      cout << "Failed resize lower" << endl;
   }
   if (e.upper( ) != 3) {
      cout << "Failed resize upper" << endl;
   }
   cout << "resize of empty vector test complete" << endl;

   cout << "Big index test" << endl;
   int low = -100000000;
   int high = low+5;
   lbvector<int> w(low,high);
   for (i=low; i<=high; i++) {
      w[i] = i;
   }
   cout << "Successfully created big index vector" << endl;
   
   for (i=low; i<=high; i++) {
      if (w[i] != i) {
         cout << "Failed Big Index test" << endl;
      }
   }

   cout << "Copy constructor test" << endl;
   lbvector<int> x(w);      // x initialized from w
   for (i=low; i<high; i++) {
      w[i] = -i;            // change original w's values
   }
   for (i=low; i<high; i++) {
      if (x[i] != i) {      // copy's values should not have changed
         cout << "Failed copy constructor test. "
              << "Copy should have a separately allocated space." << endl;
      }
   }

   cout << "Assignment operator test" << endl;
   lbvector<int> y;

   y = x;
   for (i=low; i<high; i++) {
      if (y[i] != i) {
         cout << "Failed assignment operator test" << endl;
      }
   }
   cout << "Fill test" << endl;
   y.fill(42);
   for (i=low; i<high; i++) {
      if (y[i] != 42) {
         cout << "Failed fill test" << endl;
      }
   }


   cout << "End of tests" << endl;

   return 0;
}

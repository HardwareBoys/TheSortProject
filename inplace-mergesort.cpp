#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

#define INIT 8192
#define STEP 2

using namespace std;

void exchange       (int idx1, int idx2);
void mergesort      (int, int a[]);
int lower           (int from, int to, int val);
int upper           (int from, int to, int val);
void insert_sort    (int from, int to);
int gcd             (int m, int n);
void rotateSimple         (int from, int mid, int to);
void rotateSmart_NoVector         (int from, int mid, int to);
void rotateSmart_VectMax2 (int from, int mid, int to);
void rotateSmart_VectMax4 (int from, int mid, int to);
void merge          (int from, int pivot, int to, int len1, int len2);
void sort           (int from, int to);
void sort           ();
void PrintArr       (int* arr, int size);

int * nums;
int   numsNr  = 0;
int   numsMax = INIT;

int RECURSION_DEPTH = 12;
int BETTER_ROTATION = 1;

void Print()
{
    for (int i=0; i<numsNr; i++)
        cout << nums[i] << " ";
    cout << endl;
}

bool doprint=true;
int main ( int argc, char * argv [] )
{
    switch ( argc )
    {
        case 3: 
            RECURSION_DEPTH = ( strtoul ( argv [2], NULL, 10 ) ? strtoul ( argv [2], NULL, 10 ) : 12 );
        case 2:
		{
			if (  strcmp("rot_simple", argv[1]) == 0 )
      {
            BETTER_ROTATION = 0;
            if (doprint)cout << endl <<"Algorithm: Simple rotation" << endl;
      }
      else if (  strcmp("rot_smart_novect", argv[1]) ==0 )
      {
            BETTER_ROTATION = 1;
            if (doprint)cout << endl << "Algorithm: **SMART** --NO vect -- "<< endl;
      }
      else if (  strcmp("rot_smart_vect2", argv[1]) ==0 )
      {
            BETTER_ROTATION = 2;
            if (doprint)cout << endl << "Algorithm: **SMART**  MAX 2 vectorisation!"<< endl;
      }
      else if (  strcmp("rot_smart_vect4", argv[1]) ==0 )
      {
            BETTER_ROTATION = 3;
            if (doprint)cout << endl <<"Algorithm: **SMART** UMAX 4 vectorisation !"<< endl;
      }
      else exit(-1);
		}
    }
    //printf ( "recdepth: %d better %d?\n", RECURSION_DEPTH, BETTER_ROTATION );
    nums = ( int * ) malloc ( INIT * sizeof ( int ) );

    while ( scanf ( "%d", nums + ( numsNr ++ ) ) != -1 )
    {
        if ( numsNr == numsMax )
            nums = ( int * ) realloc ( nums, ( numsMax *= STEP ) * sizeof ( int ) );
        if ( getchar () == '\n' )
            break;
    }        
    
   if (doprint) Print ();
    sort( 0, numsNr );
    if (doprint)cout << "AFTER" << endl;
   if (doprint) Print ();
    free ( nums );
}

void exchange( int idx1, int idx2 )
{
  int tmp    = nums[idx1];
  nums[idx1] = nums[idx2];
  nums[idx2] = tmp;
}

int lower (int from, int to, int val) 
{ 
    int len = to - from, half;
    while (len > 0) 
    {
        half = len/2;
        int mid= from + half;
        if ( nums [mid]  < nums[val] ) 
        {
            from = mid+1;
            len = len - half -1;
        } 
        else len = half;
    }
    return from;
}

int upper (int from, int to, int val) 
{
    int len = to - from, half;
    while (len > 0) 
    {
        half = len/2;
        int mid= from + half;
        if ( nums [val]  < nums [mid] )
            len = half;
        else 
        {
            from = mid+1;
            len = len - half -1;
        }
  }
  return from;
}

void insert_sort (int from, int to) 
{
    if (to > from+1) 
    {
        for (int i = from+1; i < to; i++) 
        {
            for (int j = i; j > from; j--) 
            {
                if (nums [j]  < nums[j-1])
                    exchange(j, j-1);
                else break;
            }
        }
  }
}

int gcd ( int m, int n ) 
{
  while ( n!=0 ) { int t = m % n; m=n; n=t; }
  return m;
}

void reverse ( int from, int to ) 
{
    while (from < to) 
        exchange( from ++, to -- );
}

void rotateSimple ( int from, int mid, int to) 
{
  /*  a less sophisticated but costlier version: */

  reverse(from, mid-1);
  reverse(mid, to-1);
  reverse(from, to-1); 
}

void rotateSmart_NoVector (int from, int mid, int to)
{
    if (from==mid || mid==to) return;
    int n = gcd(to - from, mid - from);
    while (n-- != 0) 
    {
        int val = nums[from+n];
        int shift = mid - from;
        int p1 = from+n, p2=from+n+shift;
        while (p2 != from + n) 
        {
            nums[p1] = nums[p2];
            p1=p2;
            if ( to - p2 > shift) p2 += shift;
            else p2=from + (shift - (to - p2));
        }
        nums[p1] = val;
    }
}

int swapVector4[4] = { 0,0,0,0 }; 

void rotateSmart_VectMax2 (int from, int mid, int to)
{
    if (from==mid || mid==to) return;
    int len1=mid-from;
    int len2=to-mid;
    int n = gcd(to - from, mid - from);
    while (n != 0) 
    {
      if (n >= 2) //swap 2
      {
        n-=2;
        swapVector4[0] = nums[from+n];
        swapVector4[1] = nums[from+n+1];

        int shift = mid - from;

        int p1_0 = from+n; 
        int p1_1 = from+n+1;

        int p2_0 = from+n+shift;
        int p2_1 = from+n+shift+1;

        while (p2_0 != from + n) 
        {

            if ( (p1_1-p1_0 ==1) && ((p2_1 - p2_0) == 1) )
            {
              nums[p1_0] = nums[p2_0];
              nums[p1_0+1] = nums[p2_0+1];
            }
            else 
            {
              nums[p1_0] = nums[p2_0];
              nums[p1_1] = nums[p2_1];
            }
            p1_0=p2_0;
            p1_1=p2_1;
            if ( to - p2_0 > shift) p2_0 += shift;
            else p2_0=from + (shift - (to - p2_0));
            if ( to - p2_1 > shift) p2_1 += shift;
            else p2_1=from + (shift - (to - p2_1));
        }
        nums[p1_0] = swapVector4[0];
        nums[p1_1] = swapVector4[1];
      }
      else //swap1
      {
        n-=1;
        int val_0 = nums[from+n];
        int shift = mid - from;
        int p1_0 = from+n;
        int p2_0 = from+n+shift;
        while (p2_0 != from + n) 
        {
            nums[p1_0] = nums[p2_0];
            p1_0=p2_0;
            if ( to - p2_0 > shift) p2_0 += shift;
            else p2_0=from + (shift - (to - p2_0));
        }
        nums[p1_0] = val_0;
      }
    }
}

void rotateSmart_VectMax4 (int from, int mid, int to)
{
    if (from==mid || mid==to) return;
    int n = gcd(to - from, mid - from);
    while (n != 0) 
    {
      if (n >= 4) //swap 4
      {
        n-=4;
        swapVector4[0] = nums[from+n];
        swapVector4[1] = nums[from+n+1];
        swapVector4[2] = nums[from+n+2];
        swapVector4[3] = nums[from+n+3];

        int shift = mid - from;

        int p1_0 = from+n; 
        int p1_1 = from+n+1;
        int p1_2 = from+n+2;
        int p1_3 = from+n+3;

        int p2_0 = from+n+shift;
        int p2_1 = from+n+shift+1;
        int p2_2 = from+n+shift+2;
        int p2_3 = from+n+shift+3;

        while (p2_0 != from + n) 
        {
            if ( (p1_3-p1_0 ==3) && ((p2_3 - p2_0) == 3) )
            {
              cout <<"runit" << endl;
              nums[p1_0] = nums[p2_0];
              nums[p1_0+1] = nums[p2_0+1];
              nums[p1_0+2] = nums[p2_0+2];
              nums[p1_0+3] = nums[p2_0+3];
            }
            else 
            {
              nums[p1_0] = nums[p2_0];
              nums[p1_1] = nums[p2_1];
              nums[p1_2] = nums[p2_2];
              nums[p1_3] = nums[p2_3];
            }
            p1_0=p2_0;
            p1_1=p2_1;
            p1_2=p2_2;
            p1_3=p2_3;
            if ( to - p2_0 > shift) p2_0 += shift;
            else p2_0=from + (shift - (to - p2_0));

            if ( to - p2_1 > shift) p2_1 += shift;
            else p2_1=from + (shift - (to - p2_1));

            if ( to - p2_2 > shift) p2_2 += shift;
            else p2_2=from + (shift - (to - p2_2));

            if ( to - p2_3 > shift) p2_3 += shift;
            else p2_3=from + (shift - (to - p2_3));
        }
        nums[p1_0] = swapVector4[0];
        nums[p1_1] = swapVector4[1];
        nums[p1_2] = swapVector4[2];
        nums[p1_3] = swapVector4[3];
      }
      else if (n >= 2) //swap 2
      {
        n-=2;
        swapVector4[0] = nums[from+n];
        swapVector4[1] = nums[from+n+1];

        int shift = mid - from;

        int p1_0 = from+n; 
        int p1_1 = from+n+1;

        int p2_0 = from+n+shift;
        int p2_1 = from+n+shift+1;

        while (p2_0 != from + n) 
        {

            if ( (p1_1-p1_0 ==1) && ((p2_1 - p2_0) == 1) )
            {
              nums[p1_0] = nums[p2_0];
              nums[p1_0+1] = nums[p2_0+1];
            }
            else 
            {
              nums[p1_0] = nums[p2_0];
              nums[p1_1] = nums[p2_1];
            }
            p1_0=p2_0;
            p1_1=p2_1;
            if ( to - p2_0 > shift) p2_0 += shift;
            else p2_0=from + (shift - (to - p2_0));
            if ( to - p2_1 > shift) p2_1 += shift;
            else p2_1=from + (shift - (to - p2_1));
        }
        nums[p1_0] = swapVector4[0];
        nums[p1_1] = swapVector4[1];
      }
      else //swap1
      {
        n-=1;
        int val_0 = nums[from+n];
        int shift = mid - from;
        int p1_0 = from+n;
        int p2_0 = from+n+shift;
        while (p2_0 != from + n) 
        {
            nums[p1_0] = nums[p2_0];
            p1_0=p2_0;
            if ( to - p2_0 > shift) p2_0 += shift;
            else p2_0=from + (shift - (to - p2_0));
        }
        nums[p1_0] = val_0;
      }
    }
}



void merge (int from, int pivot, int to, int len1, int len2) 
{
  if (len1 == 0 || len2==0) return;
  if (len1+len2 == 2) 
  {
    if (nums [pivot]  < nums[from])
    exchange(pivot, from);
    return;
  }
  int first_cut, second_cut;
  int len11, len22;
  if (len1 > len2) 
  {
    len11=len1/2;
    first_cut = from + len11;
    second_cut = lower(pivot, to, first_cut);
    len22 = second_cut - pivot;
  } 
  else 
  {
    len22 = len2/2;
    second_cut = pivot + len22;
    first_cut = upper(from, pivot, second_cut);
    len11=first_cut - from;
  }
  
  if ( BETTER_ROTATION==0) 
    rotateSimple(first_cut, pivot, second_cut);
  else if (BETTER_ROTATION==1)
    rotateSmart_NoVector(first_cut, pivot, second_cut);
  else if (BETTER_ROTATION==2)
    rotateSmart_VectMax2(first_cut, pivot, second_cut);
  else if (BETTER_ROTATION==3)
    rotateSmart_VectMax4(first_cut, pivot, second_cut);
  else
  {
    cout << "Invalid rotation code" << endl;
    exit(-1);
  }
  int new_mid=first_cut+len22;
  merge(from, first_cut, new_mid, len11, len22);
  merge(new_mid, second_cut, to, len1 - len11, len2 - len22);
}
 
void sort(int from, int to) 
{
    if (to - from < RECURSION_DEPTH ) 
    {
        insert_sort (from, to);
        return;
    }
    int middle = (from + to)/2;
    sort (from, middle);
    sort (middle, to);
    merge(from, middle, to, middle-from, to - middle);
 }
 

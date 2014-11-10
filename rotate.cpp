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
void rotate         (int from, int mid, int to);
void rotate2         (int from, int mid, int to);
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

int main ( int argc, char * argv [] )
{
    switch ( argc )
    {
        case 3: 
            RECURSION_DEPTH = ( strtoul ( argv [2], NULL, 10 ) ? strtoul ( argv [2], NULL, 10 ) : 12 );
        case 2:
		{
			if ( argv [1] == "worse" )
             BETTER_ROTATION = 0;
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
    cout << "better rotation = " << BETTER_ROTATION << endl;
    cout << "depth " << RECURSION_DEPTH << endl;
    sort( 0, numsNr );
    //Print ();
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

void rotate2 ( int from, int mid, int to) 
{
  /*  a less sophisticated but costlier version: */
  reverse(from, mid-1);
  reverse(mid, to-1);
  reverse(from, to-1); 
}

void rotate (int from, int mid, int to)
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
  
  BETTER_ROTATION ? rotate(first_cut, pivot, second_cut) : rotate(first_cut, pivot, second_cut) ;
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
 

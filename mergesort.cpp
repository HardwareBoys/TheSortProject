/* 
 * File:   main.cpp
 * Author: borec
 *
 * Created on 23. říjen 2014, 0:31
 */

#include <stdio.h>
#include <stdlib.h>
#define INIT 8192
#define STEP 2

void merge( int * a, int low, int high, int mid)
{
    int i, j, k; 
    int * c = (int *) malloc ( ( high + 1 ) * sizeof ( int ) );
    i = low;
    k = low;
    j = mid + 1;
    while (i <= mid && j <= high)
    {
        if (a[i] < a[j])
        {
            c[k] = a[i];
            k++;
            i++;
        }
        else
        {
            c[k] = a[j];
            k++;
            j++;
        }
    }

    while (i <= mid)
    {
        c[k] = a[i];
        k++;
        i++;
    }

    while (j <= high)

    {
        c[k] = a[j];
        k++;
        j++;
    }
    for (i = low; i < k; i++)
        a[i] = c[i];
    free ( c );
}


void mergesort( int * nums, int from, int to )
{
    int mid;
    if (from < to)
    {
        mid = ( from + to ) / 2;
        mergesort ( nums, from,     mid );
        mergesort ( nums, mid + 1,  to  );
        merge     ( nums, from, to, mid );
    }
    return;
}

int main () 
{
    int * nums;
    int   numsNr  = 0;
    int   numsMax = INIT;
   
    nums = ( int * ) malloc ( INIT * sizeof ( int ) );

    while ( scanf ( "%d", nums + ( numsNr ++ ) ) != -1 )
    {         
        if ( numsNr == numsMax )
            nums = ( int * ) realloc ( nums, ( numsMax *= STEP ) * sizeof ( int ) );
        if ( getchar () == '\n' )
            break;
    }        
    
    mergesort ( nums, 0, numsNr - 1 );
/*    
    for ( int i = 0; i < numsNr; i ++ )
        printf ( "%d ", nums [i] );
    printf ( "\n" );
*/      
	free ( nums );
    return 0;
}


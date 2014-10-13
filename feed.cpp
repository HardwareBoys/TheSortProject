/* 
 * File:   main.cpp
 * Author: borec
 *
 * Created on 30. září 2014, 20:52
 */

#include <cstdlib>
#include <cstdio>
#include <random>
#include <time.h>

using namespace std;

const static int N = 1000000, R = 1000;

int main ( int argc, char ** argv ) 
{
    random_device rd;
    unsigned long n = N, r = R;
    
    switch ( argc )
    {
        case 3:   
        {
            if ( ! ( r = strtoul ( argv [2], NULL, 10 ) ) )
                r = R;
        }
        case 2:
        {
            if ( ! ( n = strtoul ( argv [1], NULL, 10 ) ) )
                n = N;
        }    
    }
    
    srand ( rd () );
    printf ("%lu ", n);
    for ( unsigned long i = 1; i < n; i ++ )
        printf ( "%lu ", rand () % r ); 
    printf ( "%lu\n", rand () % r );
    return 0;
}


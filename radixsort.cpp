/* 
 * File:   main.cpp
 * Author: borec
 *
 * Created on 16. říjen 2014, 12:45
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define INIT 8192
#define STEP 2

int main ( int argc, char ** argv ) 
{
    int   BASE = 10;
    int   ASC  = 1;
    int   VRB  = 0; 
    
    int   ib, in, i, id;
    int   max  = 0, maxd = 1;
    
    int ** buckets;
    int  * bucketsFill;
    int  * bucketsMax;
    
    int * nums;
    int   numsNr  = 0;
    int   numsMax = INIT;
    
    for ( i = 1; i < argc; i ++ )
    {
        if ( argv [i][0] == '-' )
		{
	    	switch ( argv [i][1] )
			{
				case 'b' :
				{
					if ( BASE = strtoul ( argv [i] + 2, NULL, 10 ) && BASE != 1 );
					else BASE = 10;
					break;					
				}
				case 'r' :
				{
					ASC = 0;
					break;
				}
				case 'v' :
				{
					VRB = 1;
					break;
				}
			}
		}
    }

    nums = ( int * ) malloc ( INIT * sizeof ( int ) );

    while ( scanf ( "%d", nums + ( numsNr ++ ) ) != -1 )
    {
        if ( max < nums [ numsNr - 1 ] )
             max = nums [ numsNr - 1 ];            
        if ( numsNr == numsMax )
            nums = ( int * ) realloc ( nums, ( numsMax *= STEP ) * sizeof ( int ) );
        if ( getchar () == '\n' )
            break;
    }        
    
    buckets     = ( int ** ) malloc ( BASE * sizeof ( int * ) );
    bucketsFill = ( int *  ) malloc ( BASE * sizeof ( int   ) );
    bucketsMax  = ( int *  ) malloc ( BASE * sizeof ( int   ) );
    
    while ( ( max /= BASE ) > 0 ) maxd ++; 
    
    for ( ib = 0; ib < BASE; ib ++ )
    {
        bucketsMax  [ib] = ( numsNr / BASE + 1 );
        buckets     [ib] = ( int * ) malloc ( bucketsMax [ib] * sizeof ( int ) );  
    }
    
    for ( id = 0; id < maxd; id ++ )
    {
        for ( ib = 0; ib < BASE; ib ++ )
            bucketsFill [ib] = 0;

        for ( in = 0; in < numsNr; in ++ )
        {
            ib = nums [in];
            for ( int i = 0; i < id; i ++ )
                ib /= BASE; 
            ib %= BASE;
            
            buckets [ib] [ bucketsFill [ib] ++ ] = nums [in];
            if ( bucketsFill [ib] == bucketsMax [ib] )
                buckets [ib] = ( int * ) realloc ( buckets [ib], ( bucketsMax [ib] *= STEP ) * sizeof ( int ) );
        }
        in = 0;
        
        for ( ASC ? ib = 0 : ib = BASE - 1 ; ASC ? ib < BASE : ib >= 0; ASC ? ib ++ : ib -- )
            for ( i = 0; i < bucketsFill [ib]; i ++ )
                nums [ in ++ ] = buckets [ib][i];
    }
	if ( VRB )
	{        
    	for ( i = 0; i < numsNr; i ++ )
    	    printf ( "%d ", nums [i] );
    	printf ( "\n" );
 	}     
    free ( nums );
    for ( ib = 0; ib < BASE; ib ++ )
        free ( buckets [ib] );
    free ( buckets );
    free ( bucketsFill );
    free ( bucketsMax  );
    return 0;
}


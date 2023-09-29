#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "io.h"
#include "C:/Users/mvideo/Desktop/TX/TXLib.h"

int main( void )
{
    Stack stack = {};

    STACK_CTOR( &stack );

    StackElem a = 0;

    StackPop( &stack, &a );

    for( int i = 0; i < 100000; i++ )
    {
        if( i > 63000 )
        {
            printf( "%d %zu\n", i, stack.capacity );
        }

        StackPush( &stack, 99 + i );
    }

    STACK_DUMP( &stack );
}
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

    STACK_DUMP( &stack );

    StackPop( &stack, &a );

    for( int i = 0; i < 4; i++ )
    {
        for( ; ; )
        {
            STACK_DUMP( &stack );

            StackPush( &stack, 99 + i );
        }      
    }

    for( int i = 0; i < 5; i++ )
    {
        StackElem x = 0;
        
        StackPush( &stack, 99 );

        StackPop( &stack, &x );
    }

    StackDtor( &stack );
}
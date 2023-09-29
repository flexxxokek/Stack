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

    for( int i = 0; i < 4; i++ )
    {
        STACK_DUMP( &stack );

        StackPush( &stack, 99 + i );
    }

    for( int i = 0; i < 5; i++ )
    {   
        StackPush( &stack, 99 + i );

        STACK_DUMP( &stack );

        StackPop( &stack, nullptr );

        StackPush( &stack, 99 );
    }

    StackPush( &stack, 99 );

    StackPop( &stack, nullptr );

    STACK_DUMP( &stack );
}
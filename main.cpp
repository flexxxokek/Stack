#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "io.h"
#include "C:/Users/mvideo/Desktop/TX/TXLib.h"

#define StackCtor( stack )      \
StackCtor( #stack, __FILE__,    \
           __func__, __LINE__ );\

int main( void )
{
    Stack* stack = StackCtor( stack );

    StackElem a = 0;

    StackPop( stack, &a );

    for( int i = 0; i < 6; i++ )
    {
        StackPush( stack, 99 + i );
    }

    STACK_DUMP( stack );

    for( int i = 0; i < 5; i++ )
    {
        StackElem x = 0;

        StackPop( stack, &x );
    }

    *( ( unsigned long long* ) stack->data - 1 ) = 57;

    StackDtor( stack );

    STACK_DUMP( stack );
}
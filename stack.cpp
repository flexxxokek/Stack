#include <stdlib.h>

#include "stack.h"

void* pushVoidPtr( void* ptr, int nbytes )
{
    return ( void* ) ( ( char* ) ptr + nbytes );
}

int HashCalc( const char* stack, size_t size )
{
    unsigned long long hash = ( unsigned long long ) stack;

    for( unsigned int i = 0; i < size; i++ )
    {
        hash += ( hash << 5 ) + stack[i];
    }

    return hash;
}

struct Stack* StackCtor( const char* varName, const char* fileName,
                         const char* funcName, const size_t line )
{
    struct Stack* stack = ( Stack* ) calloc( 1, sizeof( struct Stack ) );

    stack->leftChicken = CHIKEN_DEFAULT_NUM;

    stack->rightChicken = CHIKEN_DEFAULT_NUM;

    stack->varName = varName;

    stack->fileName = fileName;

    stack->funcName = funcName;

    stack->line = line;

    stack->size = 0;

    stack->capacity = 1;

    stack->data = ( StackElem* ) ( calloc( 
                  1, sizeof( StackElem ) +
                  sizeof( unsigned long long ) * 2 ) );

    *( unsigned long long* ) stack->data = CHIKEN_DEFAULT_NUM;

    stack->data = ( StackElem* ) ( ( ( unsigned long long* ) stack->data ) + 1 );

    *( unsigned long long* ) pushVoidPtr( 
                             stack->data, sizeof( StackElem ) * stack->capacity ) = CHIKEN_DEFAULT_NUM;

    return stack;
}

#define ERR_CASE( error )                                   \
case error:                               \
    printf( "ERROR %d " #error "\n", error );               \
                                                            \
    return error;   

STACK_ERRORS PrintError( enum STACK_ERRORS error )
{
    SET_RED_COLOR;

    switch( error )
    {
    ERR_CASE( DATA_PTR_IS_NULL )

    ERR_CASE( CAPACITY_IS_LOWER_THAN_NULL )

    ERR_CASE( SIZE_IS_GREATER_THAN_CAPACITY )

    ERR_CASE( LEFT_CHIKEN_HAS_FALLEN )

    ERR_CASE( RIGHT_CHIKEN_HAS_FALLEN )

    ERR_CASE( DATA_LEFT_CHIKEN_HAS_FALLEN )

    ERR_CASE( DATA_RIGHT_CHIKEN_HAS_FALLEN )

    default:
        SET_DEFAULT_COLOR;

        return ALLRIGHT;
    }
}

#undef ERR_CASE

STACK_ERRORS StackDtor( struct Stack* stack )
{
    if( STACK_ERRORS error = StackVerify( stack ) )
    {
        PrintError( error );

        STACK_DUMP( stack );

        return error;
    }

    for( int i = 0; i < stack->size; i++ )
        stack->data = 0;
    
    stack->capacity = -1;
    stack->size = -1;

    free( stack->data );
    free( stack );

    return ALLRIGHT;
}

STACK_ERRORS StackResize( struct Stack* stack )
{
    if( stack->size == stack->capacity )
    {
        stack->capacity *= 2;
    }
    else if( stack->size < stack->capacity / 3 )
    {
        stack->capacity /= 2;
    }

    stack->data = ( StackElem* ) realloc( 
                                 ( ( unsigned long long* ) stack->data ) - 1,
                                 sizeof( StackElem ) * stack->capacity + sizeof( unsigned long long ) * 2 );

        *( unsigned long long* ) stack->data = CHIKEN_DEFAULT_NUM;

        stack->data = ( StackElem* ) ( ( unsigned long long* ) stack->data + 1 );

        *( unsigned long long* ) pushVoidPtr( 
                                 stack->data, sizeof( StackElem ) * stack->capacity ) = CHIKEN_DEFAULT_NUM;

    return StackVerify( stack );
}

STACK_ERRORS StackVerify( const struct Stack* stack )
{
    if( stack->size < 0 ) return SIZE_IS_LOWER_THAN_NULL;
    if( stack->size > stack->capacity ) return SIZE_IS_GREATER_THAN_CAPACITY;
    if( stack->data == NULL ) return DATA_PTR_IS_NULL;

    if( stack->leftChicken != CHIKEN_DEFAULT_NUM ) return LEFT_CHIKEN_HAS_FALLEN;
    if( stack->leftChicken != CHIKEN_DEFAULT_NUM ) return RIGHT_CHIKEN_HAS_FALLEN;

    if( *( ( unsigned long long* ) stack->data - 1 ) != CHIKEN_DEFAULT_NUM ) return DATA_LEFT_CHIKEN_HAS_FALLEN;
    if( *( ( unsigned long long* ) ( stack->data + stack->capacity ) ) != CHIKEN_DEFAULT_NUM ) return DATA_RIGHT_CHIKEN_HAS_FALLEN;

    if( stack->size == stack->capacity || stack->size < stack->capacity / 3 )
        StackResize( ( Stack* ) stack );
    
    return ALLRIGHT;
}

STACK_ERRORS StackPop( struct Stack* stack, StackElem* elem )
{
    if( stack->size > 0 )
    {
        *elem = stack->data[--stack->size];
        
        stack->data[stack->size] = 0;

        return ALLRIGHT;
    }
    else
    {
        SET_RED_COLOR;

        printf( "STACK HAS NO ELEMENTS TO POP\n" );

        STACK_DUMP( stack );

        SET_DEFAULT_COLOR;
    }

    return ALLRIGHT;
}

STACK_ERRORS StackPush( struct Stack* stack, StackElem elem )
{
    if( STACK_ERRORS error = StackVerify( stack ) )
    {
        STACK_DUMP( stack );

        return error;
    }

    stack->data[stack->size] = elem;

    stack->size++;

    return ALLRIGHT;
}

STACK_ERRORS PrintStack( const struct Stack* stack, const char* fileName,
                         const char* funcName, const size_t line )
{
    SET_GRAY_COLOR;

    printf( "Called from %s in function: \"%s\", line: %zu \n", fileName, funcName, line );

    printf( "Variable \"%s\" from %s in function: \"%s\", line: %zu \n", 
            stack->varName, stack->fileName, stack->funcName, stack->line );        

    printf( "{\n" );

    printf( "\tCapacity = %zu\n", stack->capacity );

    printf( "\tSize = %zu\n", stack->size );

    printf( "\tStack ptr = %p\n", stack->data );

    SET_GREEN_COLOR;

    printf( "\tStack elements:\n" "\t{\n" );

    for( int i = 0; i < stack->capacity; i++ )
    {                 
        if( i < stack->size )                                           
            printf( "\t\t" "*[%d] = %d\n", i, stack->data[i] ); 
        else
            printf( "\t\t" " [%d] = %d\n", i, stack->data[i] );
    }  
    
    printf( "\t}\n\n" );
    
    SET_GRAY_COLOR;

    printf( "}\n" );   

    SET_DEFAULT_COLOR;

    return ALLRIGHT;
}

#include <stdlib.h>

#include "stack.h"

void* pushVoidPtr( void* ptr, int nbytes )
{
    return ( void* ) ( ( char* ) ptr + nbytes );
}

static long long HashCalc( const void* stack, size_t size )
{
    long long hash = 5483;

    for( size_t i = 0; i < size; i++ )
    {
        hash += ( hash << 5 ) + * ( ( char* ) stack + i );
    }

    return hash;
}

static void HashRefresh( struct Stack* stack )
{
    stack->hash = 0;

    long long newHash = HashCalc( stack, sizeof( *stack ) );

    stack->hash = newHash;
}


static bool HashCheck( struct Stack* stack )
{
    long long oldHash = stack->hash;

    stack->hash = 0;

    long long newHash = HashCalc( stack, sizeof( *stack ) );

    if( newHash != oldHash )
    {
        return false;
    }

    stack->hash = newHash;

    return true;
}

static void PrintFileinf( const struct Stack* stack, const char* fileName,
                         const char* funcName, const size_t line )
{
    printf( "Called from %s in function: \"%s\", line: %zu \n", fileName, funcName, line );

    printf( "Variable \"%s\" from %s in function: \"%s\", line: %zu \n", 
            stack->varName, stack->fileName, stack->funcName, stack->line );   
}

void StackCtor( Stack* stack, const char* varName, const char* fileName,
                         const char* funcName, const size_t line )
{
    stack->leftChicken = LEFT_CHIKEN_DEFAULT_NUM;

    stack->rightChicken = RIGHT_CHICKEN_DEFAULT_NUM;

    stack->varName = varName;

    stack->fileName = fileName;

    stack->funcName = funcName;

    stack->line = line;

    stack->size = 0;

    stack->capacity = STACK_DEFAULT_CAPACITY;

    StackElem* dp = ( StackElem* ) ( calloc( 1, sizeof( StackElem ) * STACK_DEFAULT_CAPACITY +
                                             sizeof( unsigned long long ) * 2 ) );

    if( dp == nullptr )
    {   
        SET_RED_COLOR;

        perror( "CALLOC ERROR" );

        SET_DEFAULT_COLOR;

        dp = nullptr;

        stack->data = nullptr;
    }
        
    stack->data = dp;

    *( unsigned long long* ) stack->data = LEFT_CHIKEN_DEFAULT_NUM;

    stack->data = ( StackElem* ) ( ( unsigned long long* ) stack->data + 1 );

    *( unsigned long long* ) ( stack->data + stack->capacity ) = RIGHT_CHICKEN_DEFAULT_NUM;

    HashRefresh( stack );

    return;
}

#define ERR_CASE( error )                                   \
case error:                                                 \
    printf( "ERROR %d " #error "\n", error );               \
                                                            \
    return error;  

#define FATAL_ERR_CASE( error )                                                                 \
case error:                                                                                     \
    printf( "FATAL ERROR %d " #error ", CUTTING OFF THE PROGRAMM...\n", error );                \
                                                                                                \
    exit( error );  

STACK_ERRORS PrintError( enum STACK_ERRORS error )
{
    SET_RED_COLOR;

    switch( error )
    {
    ERR_CASE( DATA_PTR_IS_NULL )

    ERR_CASE( SIZE_IS_LOWER_THAN_NULL )

    ERR_CASE( CAPACITY_IS_LOWER_THAN_NULL )

    ERR_CASE( SIZE_IS_GREATER_THAN_CAPACITY )

    FATAL_ERR_CASE( LEFT_CHIKEN_HAS_FALLEN )

    FATAL_ERR_CASE( RIGHT_CHIKEN_HAS_FALLEN )

    FATAL_ERR_CASE( DATA_LEFT_CHIKEN_HAS_FALLEN )

    FATAL_ERR_CASE( DATA_RIGHT_CHIKEN_HAS_FALLEN )

    FATAL_ERR_CASE( HASH_HAS_FALLEN )

    default:
        SET_DEFAULT_COLOR;

        return ALLRIGHT;
    }
}

#undef FATAL_ERR_CASE

#undef ERR_CASE

STACK_ERRORS StackDtor( struct Stack* stack )
{
    if( STACK_ERRORS error = StackVerify( stack ) )
    {
        STACK_DUMP( stack );

        PrintError( error );

        putchar( '\n' );

        return error;
    }

    for( int i = 0; i < stack->size; i++ )
        stack->data[i] = 0;
    
    stack->capacity = -1;

    stack->size = -1;

    stack->leftChicken = LEFT_CHIKEN_DEFAULT_NUM;

    stack->rightChicken = RIGHT_CHICKEN_DEFAULT_NUM;

    stack->varName = nullptr;

    stack->fileName = nullptr;

    stack->funcName = nullptr;

    stack->line = -1;

    free( ( unsigned long long* ) stack->data - 1 );

    return ALLRIGHT;
}

STACK_ERRORS StackResize( struct Stack* stack )
{   
    if( STACK_ERRORS error = StackVerify( stack ) )
    {
        STACK_DUMP( stack );

        PrintError( error );

        putchar( '\n' );

        return error;
    }
    
    if( stack->size == stack->capacity )
    {
        stack->capacity *= 2;
    }
    else if( stack->size < stack->capacity / 3 )
    {
        stack->capacity /= 2;
    }

    HashRefresh( stack );

    StackElem* dp = ( StackElem* ) realloc( ( unsigned long long* ) stack->data - 1,
                                   sizeof( StackElem ) * stack->capacity + sizeof( unsigned long long ) * 2 );

    if( dp == nullptr )
    {
        perror( "CALLOC ERROR" );

        return NOT_ENOUGH_MEMORY;
    }

    stack->data = dp;

    *( unsigned long long* ) stack->data = LEFT_CHIKEN_DEFAULT_NUM;

    stack->data = ( StackElem* ) ( ( unsigned long long* ) stack->data + 1 );

    *( unsigned long long* ) ( stack->data + stack->capacity ) = RIGHT_CHICKEN_DEFAULT_NUM;

    HashRefresh( stack );

    if( STACK_ERRORS error = StackVerify( stack ) )
    {
        STACK_DUMP( stack );

        PrintError( error );

        putchar( '\n' );

        return error;
    }

    return ALLRIGHT;
}

STACK_ERRORS StackVerify( const struct Stack* stack )
{
    if( stack->size < 0 ) return SIZE_IS_LOWER_THAN_NULL;
    if( stack->size > stack->capacity ) return SIZE_IS_GREATER_THAN_CAPACITY;
    if( stack->data == nullptr ) return DATA_PTR_IS_NULL;

    if( stack->leftChicken != LEFT_CHIKEN_DEFAULT_NUM ) return LEFT_CHIKEN_HAS_FALLEN;
    if( stack->rightChicken != RIGHT_CHICKEN_DEFAULT_NUM ) return RIGHT_CHIKEN_HAS_FALLEN;

    if( *( ( unsigned long long* ) stack->data - 1 ) != LEFT_CHIKEN_DEFAULT_NUM ) return DATA_LEFT_CHIKEN_HAS_FALLEN;
    if( *( ( unsigned long long* ) ( stack->data + stack->capacity ) ) != RIGHT_CHICKEN_DEFAULT_NUM ) return DATA_RIGHT_CHIKEN_HAS_FALLEN;

    if( !HashCheck( ( Stack* ) stack ) ) return HASH_HAS_FALLEN;

    return ALLRIGHT;
}

STACK_ERRORS StackPop( struct Stack* stack, StackElem* elem )
{
    if( STACK_ERRORS error = StackVerify( stack ) )
    {
        STACK_DUMP( stack );

        PrintError( error );

        putchar( '\n' );

        return error;
    }
    
    if( stack->size > 0 )
    {
        stack->size--;

        if( elem != nullptr )
            *elem = stack->data[stack->size];
        
        stack->data[stack->size] = 0;
    }
    else
    {
        SET_RED_COLOR;

        printf( "STACK HAS NO ELEMENTS TO POP\n" );

        STACK_DUMP( stack );

        SET_DEFAULT_COLOR;
    }

    HashRefresh( stack );

    if( stack->size == stack->capacity || stack->size < stack->capacity / 3 )
    {
        StackResize( ( Stack* ) stack );

        HashRefresh( stack );
    }

    if( STACK_ERRORS error = StackVerify( stack ) )
    {
        STACK_DUMP( stack );

        PrintError( error );

        putchar( '\n' );

        return error;
    }

    return ALLRIGHT;
}

STACK_ERRORS StackPush( struct Stack* stack, StackElem elem )
{
    if( STACK_ERRORS error = StackVerify( stack ) )
    {
        STACK_DUMP( stack );

        PrintError( error );

        putchar( '\n' );

        return error;
    }

    stack->data[stack->size++] = elem;

    HashRefresh( stack );

    if( stack->size == stack->capacity || stack->size < stack->capacity / 3 )
    {
        StackResize( ( Stack* ) stack );

        HashRefresh( stack );
    }

    if( STACK_ERRORS error = StackVerify( stack ) )
    {
        STACK_DUMP( stack );

        PrintError( error );
     
        putchar( '\n' );

        return error;
    }

    return ALLRIGHT;
}

STACK_ERRORS PrintStack( const struct Stack* stack, const char* fileName,
                         const char* funcName, const size_t line )
{
    SET_GRAY_COLOR;

    PrintFileinf( stack, fileName, funcName, line );    

    printf( "{\n" );

    printf( "\tCapacity = %zu\n", stack->capacity );

    printf( "\tSize = %zu\n", stack->size );

    printf( "\tStack ptr = %p\n", stack->data );

    SET_GREEN_COLOR;

    if( stack->size <= 0 )
    {
        printf( "\tStack has no elements...\n" );
    }
    else
    {
        printf( "\tStack elements:\n" "\t{\n" );

        for( int i = 0; i < stack->capacity; i++ )
        {                 
            if( i < stack->size )                                           
                printf( "\t\t" "*[%d] = %d\n", i, stack->data[i] ); 
            else
                printf( "\t\t" " [%d] = %d\n", i, stack->data[i] );
        }  
        
        printf( "\t}\n\n" );
    }
    
    SET_GRAY_COLOR;

    printf( "}\n" );   

    SET_DEFAULT_COLOR;

    return ALLRIGHT;
}

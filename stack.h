#include <stdio.h>

#define SET_RED_COLOR printf( "\x1b[31;1m" )
#define SET_GREEN_COLOR printf( "\x1b[32;1m" )
#define SET_GRAY_COLOR printf( "\x1b[37;1m")
#define SET_DEFAULT_COLOR printf( "\x1b[0m" )

#define STACK_DUMP( stack )                         \
PrintStack( stack, __FILE__, __func__, __LINE__ );           

typedef int StackElem;

const unsigned long long CHIKEN_DEFAULT_NUM = 8;

enum STACK_ERRORS
{
    DATA_PTR_IS_NULL = -99,

    SIZE_IS_LOWER_THAN_NULL = -100,

    CAPACITY_IS_LOWER_THAN_NULL = -101,

    SIZE_IS_GREATER_THAN_CAPACITY = -102,

    LEFT_CHIKEN_HAS_FALLEN = -103,

    RIGHT_CHIKEN_HAS_FALLEN = -104,

    DATA_LEFT_CHIKEN_HAS_FALLEN = -105,

    DATA_RIGHT_CHIKEN_HAS_FALLEN = -106,

    ALLRIGHT = 0
};

struct Stack
{
    unsigned long long int leftChicken;

    StackElem* data;

    size_t size;

    size_t capacity;

    unsigned long long hash;

    const char* varName;

    const char* fileName;

    const char* funcName;

    size_t line;

    unsigned long long int rightChicken;
};

void* pushVoidPtr( void* ptr, int nbytes );

struct Stack* StackCtor( const char* varName, const char* fileName,
                        const char* funcName, size_t line );

STACK_ERRORS StackPop( struct Stack* stack, StackElem* elem );

STACK_ERRORS StackPush( struct Stack* stack, StackElem elem );

STACK_ERRORS StackVerify( const struct Stack* stack );

STACK_ERRORS PrintStack( const struct Stack* stack, const char* fileName,
                         const char* funcName, const size_t line );

STACK_ERRORS StackDtor( struct Stack* stack );


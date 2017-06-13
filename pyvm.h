
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <string.h>
/*
typedef struct pyobject_t {
    uint8_t type;

       uint8_t u8;
        int8_t i8;
        uint32_t u32;
        int32_t i32;  int value;
        void *ptr;

}pyobject ;


typedef struct STACK_t {
    int top;
    int size;
    pyobject *stack;
} STACK;      */

char r_byte(FILE *p);
long r_value(int n, FILE *p);
static char * r_bytes(int n, FILE *p);
void pushmain(void * data, int *top, void **execstack, int *stacksize);
void * popmain(void **execstack, int *top);
void pushlocal(void *data, int *topl, void **localstack, int *namesize);
void * poplocal(void **localstack, int *topl);   /*
STACK * create_new_stack(int size);  */





    




#include "pyvm.h"
#include "opcod.h"
#include "operation.h"

#define SIZEOF_LONG 8
typedef struct pyobject_t{
    int type;
    int value;
    char *string;
}pyobject;

typedef struct STACK_t{
    int top;
    int size;
    pyobject *stack;
}STACK;
STACK * create_new_stack(int size)
{
    STACK *new = (STACK *) malloc(sizeof(STACK));
    new->top = -1;
    new->size = size; 
    new->stack = (pyobject *) malloc(size * sizeof(pyobject));
    return new;
}

int main(int argc, char **argv)
{
    FILE *fp;
    static int type, i, stacksize, codesize, constsize, namesize, varnamesize, freevarsize, cellvarsize,  \
               argcount, nlocals, filenamesize, mnamesize, firstlineno, top, topl;






















    void **execstack, **localstack;
    
    char  *filename, *name;
    int stringsize, x, y, z;



    if((argc < 2) || (argc > 2)) { 
        perror("usage: enter file name, or too many arguments"); 
        exit(1); }
    printf("Python 2.7.12 *************************************************************\n");

    fp = fopen(argv[1], "rb");
    if(fp == NULL) { 
        printf("can't open file %s\n", argv[1]); 
        exit(1); }
    
    while((type = r_byte(fp)) != 'c') {
        ;
    } 
    argcount = (int) r_value(4, fp);  printf("argcount = %d\n", argcount);
    nlocals = (int) r_value(4, fp);  printf("nlocals = %d\n", nlocals);
    stacksize = (int) r_value(4, fp);  printf("stacksize =  %d\n", stacksize);
    
    while((type = r_byte(fp)) != 'S') {
        ;
    }
    r_byte(fp);
    constsize = r_value(4, fp);
   STACK *consts = create_new_stack(constsize);

    if(consts == NULL) {
        perror("consts alloc error");
        exit(1);
    }   


    for(i = 0; i < constsize; i++) {
        type = r_byte(fp);
        if(type == 'i') {

       consts->stack[i].type = 'i';
       consts->stack[i].value = r_value(4, fp);           
            
        } 
        if(type == 'N') {

           consts->stack[i].type = 'N';
         consts->stack[i].value = 0;
            
        }
    }  printf("consts (\n"); for(i = 0; i < constsize; i++ ) { printf("%d\n ", consts->stack[i].value); }

    
    r_byte(fp);         
    namesize = (int) r_value(4, fp);

    STACK *names = create_new_stack(namesize); 



    for(i = 0; i < namesize; i++) {
        names->stack[i].type = (int) r_byte(fp); 
        stringsize = (int) r_value(4, fp);

        names->stack[i].string = r_bytes(stringsize, fp);

    }  printf("names\n"); for(i = 0; i < namesize; i++) { printf("%s\n", names->stack[i].string);}
   
        
       
    r_byte(fp);
    varnamesize = (int) r_value(4, fp);  printf("varname %s\n", (varnamesize == 0 ? "()" : "yes var"));
    r_byte(fp);
    freevarsize = (int) r_value(4, fp);  printf("freevar %s\n", (freevarsize == 0 ? "()" : " yes freevar"));
    r_byte(fp);        
    cellvarsize = (int) r_value(4, fp); printf("cellvar %s\n", (cellvarsize == 0 ? "()" : "yes cellvar"));


    if((type = r_byte(fp)) == 's') {
        filenamesize = (int) r_value(4, fp);
        filename = (char *) malloc(filenamesize * sizeof(char));
        filename = r_bytes(filenamesize, fp);  printf("filename: %s\n", filename);
    }


    if((type = r_byte(fp)) == 't') {
        mnamesize = (int) r_value(4, fp);
        name = (char *) malloc(mnamesize * sizeof(char));
        name = r_bytes(mnamesize, fp);  printf("name : %s\n", name);
    }

    firstlineno = (int) r_value(4, fp); printf("firstlineno : %d\n", firstlineno);
    
    
    printf("Executing %s............................................................\n", filename);
    
    execstack = (void **) malloc(stacksize * sizeof(void *));
    localstack = (void **) malloc(namesize * sizeof(void *));
    top = -1;
    topl = -1;

    if(fseek(fp, 0, SEEK_SET) < 0) {
        printf("%s fseek error @ 99\n", filename);
    }

    while((type = r_byte(fp)) != 's') {
        ;
    }
    codesize = (int) r_value(4, fp);  printf("%d\n", codesize);


    for(i = 54; i < codesize; i++) {
        type = (int) r_byte(fp);
        switch(type) {
            case LOAD_CONST :
                type = (int) r_byte(fp);
                pushmain((void *) &consts[type], &top, execstack, &stacksize);
                r_byte(fp); 
                break;
            case STORE_NAME :
                type = (int) r_byte(fp);
                pushlocal((void *) &names[type], &topl, localstack, &namesize);
                r_byte(fp);
                break;
            case LOAD_NAME :
                r_byte(fp);
                r_byte(fp);
                break;
            case BINARY_ADD :
                x = *((int *)popmain(execstack, &top));
                y = *((int *)popmain(execstack, &top));
                z = add(x , y);
                pushmain((void *) &z, &top, execstack, &stacksize);
                break;
            case PRINT_ITEM :
                printf("%d\n", *((int *)popmain(execstack, &top)));
                break;   
            case LOAD_FAST :
                r_byte(fp); r_byte(fp); r_byte(fp); 
                break;
            case PRINT_NEWLINE :
                printf("\n");
                break;
            case RETURN_VALUE :
                break;
 
                
        }
    }
    




    free(consts);
    free(names);
    free(filename);
    free(name);
    free(execstack);
    free(localstack);
    return 0;
}



 long r_value(int n, FILE *p) {
    long x = -1; 
    const unsigned char *buf;
    int nr; 
    if((buf = (const unsigned char *) r_bytes(n, p)) != NULL) { 




    x = buf[0]; 
    x |= (long) buf[1] << 8;
    x |= (long) buf[2] << 16;
    x |= (long) buf[3] << 24;
    #if SIZEOF_LONG > 4
    x |= -(x & 0x80000000L);
    #endif
    }
    return x;
}

static  char * r_bytes(int n, FILE *p){
    static char * b;
    b = (char *) malloc (n);
    fread(b, 1, n, p);
    return b;
}

char r_byte(FILE *p)
{
    char b; 
    fread(&b, 1, 1, p);
    return b;
}


void pushmain(void *data, int* top, void **execstack, int* stacksize) 
{
    if((*top) < (*stacksize)) {
        execstack[(*top)++] =  data;
    } else {
        printf("error: stack full , can't push\n");
    }
}
 
void *popmain(void **execstack, int *top)
{
    if((*top) > 0) {
        return execstack[--(*top)];
    } else {
        printf("error: stack empty\n");
    }
}
 
void pushlocal(void *data, int *topl, void **localstack, int *namesize)
{
    if((*topl) < (*namesize)) {
        localstack[(*topl)++] = data;
    } else {
        printf("error: stackl full, can't store\n");
    }
}

void *poplocal(void **localstack, int *topl)
{
    if((*topl) > 0) {
        return localstack[--(*topl)];
    } else {
        printf("error: stackl empty\n");
    }
}

    

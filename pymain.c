


#include "opcod.h"
#include "pyvm.h"
#include <string.h>
#include <math.h>
#define SIZEOF_LONG 8

typedef struct pyobject_t{
    int type;
    int value;
    char *string;
    void *ptr;
    float fl;
    int ref;
}pyobject;

typedef struct STACK_t{
    int top;
    int size;
    pyobject **stack;
}STACK; 







/*
double binarypower(double x, double y);  */
STACK * create_new_datastack(int size);
void push(STACK * u, pyobject *item);
pyobject*  pop(STACK * u);
STACK * create_new_stack(int size);
pyobject * create_new_object();
int main(int argc, char **argv)
{
    FILE *fp;
    static int type, i, stacksize, codesize, constsize, namesize, varnamesize, freevarsize, cellvarsize,  \
               argcount, nlocals, filenamesize, mnamesize, firstlineno, top, topl;



    STACK * consts, *names;




















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
    if((consts = create_new_datastack(constsize)) == NULL) {
        printf("error: stack alloc @ 90\n"); }

    if(consts == NULL) {
        perror("consts alloc error");
        exit(1);
    }   


    for(i = 0; i < constsize; i++) {
        type = r_byte(fp);
        if(type == 'i') {

       consts->stack[i]->type = 'i';
       consts->stack[i]->value = r_value(4, fp);           
            
        } 
        if(type == 'N') {

           consts->stack[i]->type = 'N';
         consts->stack[i]->value = 0;
            
        }
        if(type == 't' || type == 's') {
            stringsize = (int) r_value(4, fp);
            consts->stack[i]->type = 't';
            consts->stack[i]->string = r_bytes(stringsize, fp);
        }
    }  printf("consts (\n"); for(i = 0; i < constsize; i++ ){
            if(consts->stack[i]->type == 'i') { printf("%d\n", consts->stack[i]->value); }
            if(consts->stack[i]->type == 't') { printf("%s\n", consts->stack[i]->string); }
            if(consts->stack[i]->type == 'f') { printf("%f\n", consts->stack[i]->fl); }
        }

    
    r_byte(fp);         
    namesize = (int) r_value(4, fp);

    names = create_new_datastack(namesize); 



    for(i = 0; i < namesize; i++) {
        names->stack[i]->type = (int) r_byte(fp); 
        stringsize = (int) r_value(4, fp);

        names->stack[i]->string = r_bytes(stringsize, fp);

    }  printf("names(\n"); for(i = 0; i < namesize; i++) { printf("%s \n", names->stack[i]->string);}
   
        
       
    r_byte(fp);
    varnamesize = (int) r_value(4, fp);  printf("varname %s\n", (varnamesize == 0 ? "()" : "yes var"));
    STACK *varname = create_new_datastack(varnamesize);
    r_byte(fp);
    freevarsize = (int) r_value(4, fp);  printf("freevar %s\n", (freevarsize == 0 ? "()" : " yes freevar"));
    STACK * freevar = create_new_datastack(freevarsize);
    r_byte(fp);        
    cellvarsize = (int) r_value(4, fp); printf("cellvar %s\n", (cellvarsize == 0 ? "()" : "yes cellvar"));
    STACK * cellvar = create_new_datastack(cellvarsize);


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
    
    STACK *mainstack;
    if((mainstack = create_new_stack(stacksize)) == NULL) {
        printf("stack alloc error @ 153\n"); } 


 /*   push(mainstack, names->stack[0]); push(mainstack, names->stack[1]); push(mainstack, names->stack[2]); 
 
    pyobject* item = pop(mainstack);  printf("%s\n", item->string);  
    pyobject * item1 = pop(mainstack); printf("%s\n", item1->string);
    pop(mainstack);
    pyobject  *new = create_new_object(); new->string = "fgjh";
    new->value = 67;  push(mainstack, new); item = pop(mainstack); printf("%d %s\n", item->value, item->string);  */

    if(fseek(fp, 0, SEEK_SET) < 0) {
        printf("%s fseek error @ 99\n", filename);
    }

    while((type = r_byte(fp)) != 's') {
        ;
    }
    codesize = (int) r_value(4, fp); 
    pyobject *temp, *temp1, *temp2, *result;

    for(i = 0; i < codesize; i++) {
        type = (int) r_byte(fp);
        switch(type) {
            case NOP :
                break;
            case LOAD_CONST :
                type = (int) r_byte(fp); 
                push(mainstack, consts->stack[type]);
                r_byte(fp); 
                break;
            case STORE_NAME :
                type = r_byte(fp);
                names->stack[type]->ptr = (void*)pop(mainstack);
                push(mainstack, (pyobject *)(names->stack[type]->ptr));
                

                r_byte(fp);
                break;
            case LOAD_NAME :
                type = r_byte(fp);
                push(mainstack, (pyobject *)(names->stack[type]->ptr));
                r_byte(fp);
                break;
            case BINARY_ADD :
                result = create_new_object();
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                if(temp->type == 'i' && temp1->type == 'i') {
                    result->value = add(temp->value, temp1->value);
                    result->type = 'i'; }
                if(temp->type == 't' && temp1->type == 't') {
                    result->string = strcat(temp1->string, temp->string);
                    result->type = 't'; }
                push(mainstack, result);
                break;
            case PRINT_ITEM :
                
                result = pop(mainstack);
                if(result->type == 'i') {
                    printf("%d\n", result->value); }
                if(result->type == 't') {
                    printf("%s", result->string); }
                break;   
            case LOAD_FAST :
                type = r_byte(fp);
                push(mainstack, varname->stack[type]);  
                break;
            case PRINT_NEWLINE :
                printf("\n");
                break;
            case RETURN_VALUE :
                return pop(mainstack);
                break;
            case DELETE_NAME :
                type = r_byte(fp);
                if((names->stack[type]->ref) == 0) {
                    free(names->stack[type]); }
                    r_byte(fp);
                break;     
            case LOAD_GLOBAL :
                type = r_byte(fp);
                push(mainstack, names->stack[type]);
                break;
            case ROT_TWO :
     
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                push(mainstack, temp);
                push(mainstack, temp1);
                break;
            case ROT_THREE :
               
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                temp2 = pop(mainstack);
                push(mainstack, temp);
                push(mainstack, temp1);
                push(mainstack, temp2);
                break;
         /*   case BINARY_POWER :
           
                result = create_new_object();
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                result->value = (int)binarypower((double)temp1->value, (double)temp->value);
                result->type = 'i';
                push(mainstack, result);
                break;  */
            case BINARY_MULTIPLY :
    
                result = create_new_object();
                temp  = pop(mainstack);
                temp1 = pop(mainstack);
                
                if(temp->type == 'i' && temp1->type == 'i') {

                    result->value = (temp1->value) * (temp->value);
                    result->type = 'i'; }
                if(temp1->type == 'i' && temp->type == 'f' ){
                    result->type = 'f';
                    result->fl = (temp1->value) * (temp->fl);}
                if(temp1->type == 'f' && temp->type == 'i') {
                    result->type = 'f';
                    result->fl = (temp1->fl) * (temp->value); }
                if(temp1->type == 'f' && temp->type == 'f') {
                    result->type = 'f';
                    result->fl = (temp1->fl) * (temp->fl); }
                if(temp1->type == 't' && temp->type == 'i') {
                    
  
                    stringsize = strlen(temp1->string);


                    stringsize = stringsize * (temp->value);
            
                    if((result->string = (char *)malloc(sizeof(char) *stringsize)) == NULL) {
                        printf("error: realloc @293\n"); }
                    for(i = 0; i < (temp->value); i++) {
                        strcat(result->string, temp1->string); }
                    result->type = 't'; }
                push(mainstack, result);
                break;

            case BINARY_DEVIDE :

                result = create_new_object();
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                if(temp1->type == 'i' && temp->type == 'i') {
                    result->type = 'i';
                    result->value = (temp1->value) / (temp->value); }
                if(temp1->type == 'i' && temp->type == 'f') {
                    result->type = 'f';
                    result->fl = (temp1->value) / (temp->fl); }
                if(temp1->type == 'f' && temp->type == 'i') {
                    result->type = 'f';
                    result->fl = (temp1->fl) / (temp->value); }
                if(temp1->type == 'f' && temp->type == 'f') {
                    result->type = 'f';
                    result->fl = (temp1->fl) / (temp->fl); }
                push(mainstack, result);
                break;
            
            case BINARY_MODULO :
                result = create_new_object();
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                if(temp1->type == 'i' && temp->type == 'i') {
                    result->type = 'i';
                    result->value = (temp1->value) % (temp->value); }
                push(mainstack, result);
                break;
            case BINARY_FLOOR_DEVIDE :
                result = create_new_object();
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                if(temp1->type == 'i' && temp->type == 'i') {
                    result->type = 'i';
                    result->value = (temp1->value) / (temp->value); }
                if(temp1->type == 'i' && temp->type == 'f' ) {
                    result->type = 'i';
                    result->value = ((temp1->value) / (int)(temp->fl)); }
                if(temp1->type == 'f' && temp->type == 'i') {
                    result->type = 'i';
                    result->value = ((int)(temp1->fl) / (temp->value)); }
                if(temp1->type == 'f' && temp->type == 'f') {
                    result->type = 'i';
                    result->value = ((int)(temp1->fl) / (int)(temp->fl)); }
                push(mainstack, result);
                break;



            case BINARY_SUBTRACT :
                result = create_new_object();
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                if(temp1->type == 'i' && temp->type == 'i') {
                    result->type = 'i';
                    result->value = (temp1->value) - (temp->value); }
                if(temp1->type == 'i' && temp->type == 'f') {
                    result->type = 'f';
                    result->fl = (temp1->value) - (temp->fl); }
                if(temp1->type == 'f' && temp->type == 'i') {
                    result->type = 'f';
                    result->fl = (temp1->fl) - (temp->value); }
                if(temp1->type == 'f' && temp->type == 'f') {
                    result->type = 'f';
                    result->fl = (temp1->fl) - (temp->fl); }
                push(mainstack, result);
                break;

            
            case INPLACE_ADD :
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                if(temp1->type == 'i' && temp->type == 'i') {
                    temp1->value = temp1->value + temp->value; }
                if(temp1->type == 'i' && temp->type == 'f') {
                    temp1->type = 'f';
                    temp1->fl = (float)(temp1->value) + temp->fl; } 
                if(temp1->type == 'f' && temp->type == 'i') {
                    temp1->fl = temp1->fl + (float)(temp->value); }
                if(temp1->type == 'f' && temp->type == 'f') {
                    temp1->fl = temp1->fl + temp1->fl; }
                if(temp1->type == 't' && temp->type == 't') {
                    temp1->string = strcat(temp1->string, temp->string); }
                push(mainstack, temp1);
                break;

            case INPLACE_SUBTRACT :
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                if(temp1->type == 'i' && temp->type == 'i') {
                    temp1->value = temp1->value + temp->value; }
                if(temp1->type == 'i' && temp->type == 'f') {
                    temp1->type = 'f';
                    temp1->fl = (float)(temp1->value) + temp->fl; }
                if(temp1->type == 'f' && temp->type == 'i') {
                    temp1->type = 'f';
                    temp1->fl = temp1->fl + (float)(temp->value); }
                if(temp1->type == 'f' && temp->type == 'f') {
                    temp1->fl = temp1->fl + temp->fl; }
                push(mainstack, temp1);
                break;




        }
    }
    




    free(consts);
    free(names);
    free(filename);
    free(name);
    free(mainstack);

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





 void push(STACK * u, pyobject *item)
{
    if(u->top < u->size) {
        u->stack[(u->top)++] = item; 
    } else {
       /* printf("error: stack full, can't push\n") */;
    }
}
pyobject* pop(STACK * u) 
{
    if(u->top > 0) {
        return u->stack[--(u->top)];
    } else {
        /* printf("error: stack empty\n") */;
    }
}

STACK * create_new_datastack(int size)
{
    int i;
    STACK *new = (STACK *) malloc (sizeof(STACK));
    new->top = 0;
    new->size = size;
    new->stack = (pyobject **) malloc(size * sizeof(pyobject*));
    for(i = 0; i < size; i++) { 
        if((new->stack[i] = (pyobject *) malloc (sizeof(pyobject))) == NULL){
            printf("error: stack alloc @ 297\n");}
    }
    return new;
}


STACK * create_new_stack(int size)
{
    STACK * new = (STACK *) malloc(sizeof(STACK));
    new->top = 0;
    new->size = size;
    new->stack = (pyobject **) malloc(size * sizeof(pyobject*));
    return new;
}


pyobject * create_new_object()
{
    pyobject *new;
    new = malloc(sizeof(pyobject));
    return new;
}
/*
double binarypower(double x, double y)
{
    return pow(x, y);
}
 */








 




























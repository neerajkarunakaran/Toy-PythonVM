#include "opcod.h"
#include "pyvm.h"
#include <string.h>
#include <math.h>
#define SIZEOF_LONG 8
#define bool int
#define true 1
#define false 0
typedef struct pyobject_t{
    int type; 
    bool  boolvalue;
    void *data;
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






 //static int * create_new_bytecode(int codesize);
/*
double binarypower(double x, double y);  */
STACK * create_new_datastack(int size);
void push(STACK * u, pyobject *item);
pyobject*  pop(STACK * u);
STACK * create_new_stack(int size);
pyobject * create_new_object();
pyobject * create_new_list(STACK *mainstack, int size);
int main(int argc, char **argv)
{
    FILE *fp;
    static int type, i, stacksize, codesize, constsize, namesize, varnamesize, freevarsize, cellvarsize,  \
               argcount, nlocals, filenamesize, mnamesize, firstlineno, top, topl;



    STACK  * consts, *names, *varname, *freevar, *cellvar;



















    char *c;
    char  *filename, *name, bytes;
    int stringsize;



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



    while((type = r_byte(fp)) != 's') {
        ;
    } 

    codesize = (int) r_value(4, fp); 
    





    c = r_bytes(codesize, fp); 








  fseek(fp, 0, SEEK_SET);
    while((type = r_byte(fp)) != 'S') {
        ;
    }

    type = r_byte(fp); 
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
    } 

    printf("consts (");
    if(constsize){
         for(i = 0; i < constsize; i++ ){
             if(consts->stack[i]->type == 'i') { printf("%d ", consts->stack[i]->value); }
             if(consts->stack[i]->type == 't') { printf("%s ", consts->stack[i]->string); }
             if(consts->stack[i]->type == 'f') { printf("%f ", consts->stack[i]->fl); }
             if(consts->stack[i]->type == 'N') { printf("%d ", consts->stack[i]->value); }
         }  
    } else {
        printf(" ");
    } printf(")\n");     
    type = r_byte(fp); 
    if(type == '(') {
 
    namesize = (int) r_value(4, fp);

    if((names = create_new_datastack(namesize)) == NULL) {printf(" alloc error @163\n");} 
    }


    for(i = 0; i < namesize; i++) {
        type = r_byte(fp);
        if(type == 't'){ 
            stringsize = (int) r_value(4, fp);
            names->stack[i]->type = 't';
            names->stack[i]->string = r_bytes(stringsize, fp); }
        if(type == 'i') {
            names->stack[i]->type = 'i';    
            names->stack[i]->value = (int) r_value(4, fp); }

    }
    printf("names(");
    if(namesize) { 
        for(i = 0; i < namesize; i++) {
            type = names->stack[i]->type;
            if(type == 't') { 
                printf("%s ", names->stack[i]->string);}
            if(type == 'i') {
                printf("%d ", names->stack[i]->value); }
        }
    } else {
        printf(" ");
    }   printf(")\n");    

       
    r_byte(fp);
    varnamesize = (int) r_value(4, fp);
    if(varnamesize) {
        varname = create_new_datastack(varnamesize); 
        for(i = 0; i < varnamesize; i++) {
            type = r_byte(fp);
            if(type == 'i') {
                varname->stack[i]->type = 'i';
                varname->stack[i]->value = (int)r_value(4, fp); }
            if(type == 't') {
                varname->stack[i]->type = 't';
                stringsize = (int) r_value(4, fp);
                varname->stack[i]->string = r_bytes(stringsize, fp); }
        }
    }
    printf("varname (");
    if(varnamesize) {
        for(i = 0; i < varnamesize; i++) {
            type = varname->stack[i]->type;
            if(type == 'i') {
                printf("%d ", varname->stack[i]->value); }
            if(type == 't') {
                printf("%s ", varname->stack[i]->string);} 
        }
    } else {
        printf(" ");
    } printf(")\n");

    r_byte(fp);
    freevarsize = (int) r_value(4, fp); 

    if(freevarsize){
        freevar = create_new_datastack(freevarsize);
        for(i = 0; i < freevarsize; i++) {
            type = r_byte(fp);
            if(type == 'i') {
                freevar->stack[i]->type = 'i';
                freevar->stack[i]->value = (int) r_value(4, fp); }
            if(type == 't') {
                stringsize = (int) r_value(4, fp);
                freevar->stack[i]->type = 't';
                freevar->stack[i]->string = r_bytes(stringsize, fp); }
        }
    } printf("freevar (");
    if(freevarsize) {
        for(i = 0; i < freevarsize; i++) {
            type = freevar->stack[i]->type;
            if(type == 'i') {
                printf("%d ", freevar->stack[i]->value); }
            if(type == 't') {
                printf("%s ", freevar->stack[i]->string); }
        }
    } else {
        printf(" ");
    } printf(")\n");
    r_byte(fp);        
    cellvarsize = (int) r_value(4, fp);
    if(cellvarsize) {
        cellvar = create_new_datastack(cellvarsize);
        for(i = 0; i < cellvarsize; i++) {
            type = r_byte(fp);
            if(type == 'i') {
                cellvar->stack[i]->type = 'i';
                cellvar->stack[i]->value = (int) r_value(4, fp); }
            if(type == 't') {
                cellvar->stack[i]->type == 't';
                stringsize = (int) r_value(4, fp);
                cellvar->stack[i]->string = r_bytes(stringsize, fp); }
        }
    } printf("cellvar (");
    if(cellvarsize) {
        for(i = 0; i < cellvarsize; i++) {
            type = cellvar->stack[i]->type;
            if(type == 'i') {
                printf("%d ", cellvar->stack[i]->value); }
            if(type == 't') {
                printf("%s ", cellvar->stack[i]->string); }
        }
    } else {
        printf(" ");
    } printf(")\n");

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



 















    pyobject *temp, *temp1, *temp2, *result;
    i = 0;
    while(i < codesize) {
        type = c[i++];  
        switch(type) {
            case NOP :
                break;
            case LOAD_CONST :  
                type = c[i]; 
                push(mainstack, consts->stack[type]); i = i + 2;
               // r_byte(fp);  
                break;
            case STORE_NAME :
                type = c[i]; 
                names->stack[type]->ptr = (void*)pop(mainstack);
                
                

                i = i + 2;
                break;
            case LOAD_NAME :
                type = c[i];
                push(mainstack, (pyobject *)(names->stack[type]->ptr)); 
                i = i + 2;
                break;

            case STORE_FAST :

                type = c[i];
                
                varname->stack[type]->ptr = (void *) pop(mainstack);
    

                i = i + 2;
                break;
                
            case LOAD_FAST :
                type = c[i];

                push(mainstack, (pyobject *)(varname->stack[type]->ptr));


                i = i + 2;
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
                    printf("%d", result->value); }
                if(result->type == 't' || type == 's') {
                    printf("%s", result->string); }
                if(result->type == 'f') {
                    printf("%f", result->fl); }
                if(result->type == 'b') {
                    if(result->boolvalue == true) {
                        printf("true");
                    } else {
                        printf("false"); }
                }
                if(result->type == '[') { 
                    temp = result;
                    printf("[");
                    while(result != NULL) {
                        temp =(pyobject *)(result->data);
                      
                        if(temp->type == 'i') {
                            printf("%d ", temp->value); }
                        if(temp->type == 'f') {
                            printf("%f ", temp->fl); }
                        if(temp->type == 't' || temp->type == 's') {
                            printf("%s ", temp->string); }
                   
                       
                        result = (pyobject *)(result->ptr);
                    }  
                    printf("]\n");
                }  
                            
                    
                break;

    
            case PRINT_NEWLINE :
                printf("\n");
                break;
 /*           case RETURN_VALUE :
                return pop(mainstack);
                break;
*/
            case DELETE_NAME :
                type = c[i];
                if((names->stack[type]->ref) == 0) {
                    free(names->stack[type]); 
                } else {
                    --(names->stack[type]->ref); }
                i = i + 2;
                break;
            case STORE_GLOBAL :
                type = c[i];
                names->stack[type]->ptr = (void*) pop(mainstack);
                i = i + 2;
                break;
            case LOAD_GLOBAL :
                type = c[i];
                push(mainstack, (pyobject *)(names->stack[type]->ptr));
                i = i + 2;
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

            case INPLACE_MULTIPLY :
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                if(temp1->type == 'i' && temp->type == 'i') {
                    temp1->type = 'i';
                    temp1->value = temp1->value * temp->value; }
                if(temp1->type == 'i' && temp->type == 'f') {
                    temp1->type = 'f';
                    temp1->fl = (float)(temp1->value) * temp->fl; }
                if(temp1->type == 'f' && temp->type == 'i') {
                    temp1->type == 'f';
                    temp1->fl = temp1->fl * (float)(temp->value); }
                if(temp1->type == 'f' && temp->type == 'f') {
                    temp1->fl = temp1->fl * temp->fl; }
                push(mainstack, temp1);
                break;
            case INPLACE_DEVIDE :
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                if(temp1->type == 'i' && temp->type == 'i') {
                    temp1->value = temp1->value / temp->value; }
                if(temp1->type == 'i' && temp->type == 'f') {
                    temp1->type == 'f';
                    temp1->fl = (float)(temp1->value) / temp->fl; }
                if(temp1->type == 'f' && temp->type == 'i') {

                    temp1->fl = temp1->fl / (float)(temp->value); }
                if(temp1->type == 'f' && temp->type == 'f') {
                    temp1->fl = temp1->fl / temp->fl; }
                push(mainstack, temp1);
                break;

            case INPLACE_MODULO :
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                if(temp1->type == 'i' && temp->type == 'i') {
                    temp1->value = temp1->value % temp->value; }
                push(mainstack, temp1);
                break;     
            case BINARY_LSHIFT :
                result = create_new_object();
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                if(temp1->type == 'i' && temp->type == 'i') {
                    result->type = 'i';
                    result->value = temp1->value << temp->value;}
                push(mainstack, result);
                break;
            case BINARY_RSHIFT :
                result = create_new_object();
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                if(temp1->type == 'i' && temp->type == 'i') {
                    result->type = 'i';
                    result->value = temp1->value >> temp->value; }
                push(mainstack, result);
                break;
            case BINARY_AND :
                result = create_new_object();
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                if(temp1->type == 'i' && temp->type == 'i') {
                    result->type = 'i';
                    result->value = temp1->value & temp->value; }
                push(mainstack, result);
                break;
            case BINARY_XOR :
                result = create_new_object();
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                if(temp1->type == 'i' && temp->type == 'i') {
                    result->type = 'i';
                    result->value = temp1->value ^ temp->value; }
                push(mainstack, result);
                break;
            case BINARY_OR :
                result = create_new_object();
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                if(temp1->type == 'i' && temp->type == 'i') {
                    result->type = 'i';
                    result->value = temp1->value | temp->value; }
                push(mainstack, result);
                break;
            case INPLACE_RSHIFT :
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                if(temp1->type == 'i' && temp->type == 'i') {
                    temp1->value = temp1->value >> temp->value; }
                push(mainstack, temp1);
                break;
            case INPLACE_LSHIFT :
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                if(temp1->type == 'i' && temp->type == 'i') {
                    temp1->value = temp1->value << temp->value; }
                push(mainstack, temp1);
                break;
            case INPLACE_AND :
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                if(temp1->type == 'i' && temp->type == 'i') {
                    temp1->value = temp1->value & temp->value; }
                push(mainstack, temp1);
                break;
            case INPLACE_OR :
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                if(temp1->type == 'i' && temp->type == 'i') {
                    temp1->value = temp1->value | temp->value; }
                push(mainstack, temp1);
                break;
            case INPLACE_XOR :
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                if(temp1->type == 'i'  && temp->type == 'i'){
                    temp1->value = temp1->value ^ temp->value; }
                push(mainstack, temp1);
                break;  
            case COMPARE_OP :
                type = c[i];
                result = create_new_object();
                result->type = 'b';
                temp = pop(mainstack);
                temp1 = pop(mainstack);
                if(type == 0) {
                    if(temp->type == 'i'){
                        if(temp1->value < temp->value){
                            result->boolvalue = true;
                        } else{
                        result->boolvalue = false;}
                    } if(temp->type == 'f') {
                        if(temp1->fl < temp->fl) {
                            result->boolvalue = true;
                        } else {
                            result->boolvalue = false;}
                    }          
                }
                if(type == 1) {
                     if(temp->type == 'i'){
                        if(temp1->value <= temp->value){
                            result->boolvalue = true; 
                        } else{ 
                        result->boolvalue = false;} 
                    } if(temp->type == 'f') {
                        if(temp1->fl <= temp->fl) {
                            result->boolvalue = true; 
                        } else {
                            result->boolvalue = false;} 
                    }        
                }
                if(type == 2) {
                    if(temp->type == 'i'){
                        if(temp1->value == temp->value){
                            result->boolvalue = true; 
                        } else{ 
                        result->boolvalue = false;} 
                    } if(temp->type == 'f') {
                        if(temp1->fl == temp->fl) {
                            result->boolvalue = true; 
                        } else {
                            result->boolvalue = false;} 
                    } if(temp->type == 't' || temp->type == 's'){
                        if(strcmp(temp1->string, temp->string) == 0) {
                            result->boolvalue = true;
                        } else {
                            result->boolvalue = false; }
                    }
                }
                if(type == 3) {
                    if(temp->type == 'i'){
                        if(temp1->value != temp->value){
                            result->boolvalue = true; 
                        } else{ 
                        result->boolvalue = false;} 
                    } if(temp->type == 'f') {
                        if(temp1->fl != temp->fl) {
                            result->boolvalue = true; 
                        } else {
                            result->boolvalue = false;} 
                    } if(temp->type == 't' || temp->type == 's') {
                        if(strcmp(temp1->string, temp->string) != 0) {
                            result->boolvalue = true;
                        } else {
                            result->boolvalue = false; }
                    }
                }
                if(type == 4) {
                    if(temp->type == 'i'){
                        if(temp1->value > temp->value){
                            result->boolvalue = true; 
                        } else{ 
                        result->boolvalue = false;} 
                    } if(temp->type == 'f') {
                        if(temp1->fl > temp->fl) {
                            result->boolvalue = true; 
                        } else {
                            result->boolvalue = false;} 
                    }        
                }
                if(type == 5) {
                    if(temp->type == 'i'){
                        if(temp1->value >= temp->value){
                            result->boolvalue = true; 
                        } else{ 
                        result->boolvalue = false;} 
                    } if(temp->type == 'f') {
                        if(temp1->fl >= temp->fl) {
                            result->boolvalue = true; 
                        } else {
                            result->boolvalue = false;} 
                    }
                }        
                i = i + 2;
                push(mainstack, result);
                break;
                    
            case BUILD_LIST :
                type = c[i]; 
                result = create_new_list(mainstack, type);
                push(mainstack, result);
                break;
            case JUMP_IF_FALSE :
                type = c[i];
                result = pop(mainstack);
                if(result->type == 'b') {
                    if(result->boolvalue ==  false) {
                        i = type;
                    } else {
                        i = i + 2;
                    }
                }
                break;                                      
            default:   
                  
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
        printf("error: stack full, can't push\n");
    }
}
pyobject* pop(STACK * u) 
{
    if(u->top > 0) {
        return u->stack[--(u->top)];  
    } else {
        printf("error: stack empty\n");
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
 

 static int* create_new_bytecode(int codesize)
    int *ptr;
    if((ptr = (int *) malloc(sizeof(int)) * codesize)) == NULL) {
        printf("malloc error @695\n");
        exit(1)}
    return ptr;
}
*/

pyobject * create_new_list(STACK *mainstack, int size)
{
    int i;
    pyobject  *current, *head;
    head = NULL;
    current = NULL;
    for(i = 0; i < size; i++) {
        if(head == NULL) {
            head = create_new_object();
            head->type = '[';
            head->data = (void *)pop(mainstack);
            head->ptr = NULL; 
        } else {
            current = create_new_object(); 
            current->type = '[';
            current->data = (void *)pop(mainstack);
            current->ptr = (void*)head;
            head = current;
        }
    } 
    return  head;
}   
    




        


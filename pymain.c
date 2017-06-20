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

typedef struct codeobject_t {
    int argcount;
    int nlocals;
    int stacksize;
    int codesize;
    int constsize;
    int namesize;
    int varnamesize;
    int freevarsize;
    int cellvarsize;
    int firstlineno;
    char *filename;
    char *functionname;
    char *code;
    STACK *co_const;
    
    STACK *co_names;
    STACK *co_varname;
    STACK *co_freevar;
    STACK *co_cellvar;
 } CODEOBJECT;

typedef struct block_stack {
    int top;
    int size;
    char **stack;
}BLOCKSTACK;
static int cnt;
CODEOBJECT *codeobj;
BLOCKSTACK *blockstack;
STACK * mainstack;
BLOCKSTACK * create_new_blockstack(void); 
void call_execute(char *code);
void read_file(FILE *fp); 
CODEOBJECT * create_new_codeobj(CODEOBJECT *codeobj);  
static int codeobjcount;
 //static int * create_new_bytecode(int codesize);
/*
double binarypower(double x, double y);  */
STACK * create_new_datastack(int size);
void push(pyobject *item);
pyobject*  pop(void);
    
STACK * create_new_localstack(int size);
void pushblock(char * code);  
char * popblock(void); 
pyobject * create_new_object();
pyobject * create_new_list(int size);
int main(int argc, char **argv)
{


    FILE *fp;          
    codeobjcount = 0;
    cnt = -1;





















    





    if((argc < 2) || (argc > 2)) { 
        perror("usage: enter file name, or too many arguments"); 
        exit(1); }
    printf("****************************Python 2.7.12 ******************************\n\n");

    fp = fopen(argv[1], "rb");
    if(fp == NULL) { 
        printf("can't open file %s\n", argv[1]); 
        exit(1); }  


  //  codeobj = create_new_codeobj(); 
 //   ++codeobjcount;
  //  codeobj = create_new_codeobj();
    read_file(fp); 
    fclose(fp);
    printf("Executing %s............................................................\n", codeobj[cnt].filename);


    
    if((mainstack = create_new_localstack(codeobj[0].stacksize)) == NULL) {
        printf("stack alloc error @ 153\n"); }
   
    call_execute(codeobj[0].code);

    free(codeobj);
    free(mainstack);
    return 0;
}


void read_file(FILE *fp)
{

    static int type, stringsize, i;

    codeobj = create_new_codeobj(codeobj);
    ++codeobjcount;  
    ++cnt;

    while((type = r_byte(fp)) != 'c') {
        ;
    }

  

    codeobj[cnt].argcount = (int) r_value(4, fp); 
 //   printf("argcount = %d\n", codeobj[cnt].argcount);
    codeobj[cnt].nlocals = (int) r_value(4, fp);  
 //   printf("nlocals = %d\n", codeobj[cnt].nlocals);
    codeobj[cnt].stacksize = (int) r_value(4, fp);  
 //   printf("stacksize =  %d\n", codeobj[cnt].stacksize); 



    while((type = r_byte(fp)) != 's') {
        ;
    } 

    codeobj[cnt].codesize = (int) r_value(4, fp);  
    





    codeobj[cnt].code = r_bytes(codeobj[cnt].codesize, fp); 




 //  for(i = 0; i < codeobj[cnt].codesize; i++) { printf("%d\n", codeobj[cnt].code[i]);}








    type = r_byte(fp);
    codeobj[cnt].constsize = r_value(4, fp); 
    if((codeobj[cnt].co_const = create_new_datastack(codeobj[cnt].constsize)) == NULL) {
        printf("error: stack alloc @ 90\n"); }

    if(codeobj[cnt].co_const == NULL) {
        perror("consts alloc error");
        exit(1);
    }   


    for(i = 0; i < codeobj[cnt].constsize; i++) {
        type = r_byte(fp);
        if(type == 'i') {

        codeobj[cnt].co_const->stack[i]->type = 'i';
        codeobj[cnt].co_const->stack[i]->value = r_value(4, fp);           
            
        } 
        if(type == 'N') {

            codeobj[cnt].co_const->stack[i]->type = 'N';
            codeobj[cnt].co_const->stack[i]->value = 0;
            
        }
        if(type == 't' || type == 's') {
            stringsize = (int) r_value(4, fp);
            codeobj[cnt].co_const->stack[i]->type = 't';
            codeobj[cnt].co_const->stack[i]->string = r_bytes(stringsize, fp);
        }
        if(type == 'c') {
            codeobj[cnt].co_const->stack[i]->type = 'c';
            codeobj[cnt].co_const->stack[i]->value = codeobjcount;
            read_file(fp-3);        
        }
            
            
        
    } 
/*
    printf("consts (");
    if(codeobj[cnt].constsize){
         for(i = 0; i < codeobj[cnt].constsize; i++ ){
             if(codeobj[cnt].co_const->stack[i]->type == 'i') {
                 printf("%d ", codeobj[cnt].co_const->stack[i]->value); }
             if(codeobj[cnt].co_const->stack[i]->type == 't') 
                { printf("%s ", codeobj[cnt].co_const->stack[i]->string); }
             if(codeobj[cnt].co_const->stack[i]->type == 'f') { 
                printf("%f ", codeobj[cnt].co_const->stack[i]->fl); }
             if(codeobj[cnt].co_const->stack[i]->type == 'N') { 
                printf("%d ", codeobj[cnt].co_const->stack[i]->value); }
         }  
    } else {
        printf(" ");
    } printf(")\n");    

 */
    type = r_byte(fp); 
    if(type == '(') {
 
    codeobj[cnt].namesize = (int) r_value(4, fp);

    if((codeobj[cnt].co_names = create_new_datastack(codeobj[cnt].namesize)) == NULL) {printf(" alloc error @163\n");} 
    }


    for(i = 0; i < codeobj[cnt].namesize; i++) {
        type = r_byte(fp);
        if(type == 't'){ 
            stringsize = (int) r_value(4, fp);
            codeobj[cnt].co_names->stack[i]->type = 't';
            codeobj[cnt].co_names->stack[i]->string = r_bytes(stringsize, fp); }
        if(type == 'i') {
            codeobj[cnt].co_names->stack[i]->type = 'i';    
            codeobj[cnt].co_names->stack[i]->value = (int) r_value(4, fp); }

    }
/*
    printf("names(");
    if(codeobj[cnt].namesize) { 
        for(i = 0; i < codeobj[cnt].namesize; i++) {
            type = codeobj[cnt].co_names->stack[i]->type;
            if(type == 't') { 
                printf("%s ", codeobj[cnt].co_names->stack[i]->string);}
            if(type == 'i') {
                printf("%d ", codeobj[cnt].co_names->stack[i]->value); }
        }
    } else {
        printf(" ");
    }   printf(")\n");    
*/
      
    r_byte(fp);
    codeobj[cnt].varnamesize = (int) r_value(4, fp);
    if(codeobj[cnt].varnamesize) {
        codeobj[cnt].co_varname = create_new_datastack(codeobj[cnt].varnamesize); 
        for(i = 0; i < codeobj[cnt].varnamesize; i++) {
            type = r_byte(fp);
            if(type == 'i') {
                codeobj[cnt].co_varname->stack[i]->type = 'i';
                codeobj[cnt].co_varname->stack[i]->value = (int)r_value(4, fp); }
            if(type == 't') {
                codeobj[cnt].co_varname->stack[i]->type = 't';
                stringsize = (int) r_value(4, fp);
                codeobj[cnt].co_varname->stack[i]->string = r_bytes(stringsize, fp); }
        }
    }

/*
    printf("varname (");
    if(codeobj[cnt].varnamesize) {
        for(i = 0; i < codeobj[cnt].varnamesize; i++) {
            type = codeobj[cnt].co_varname->stack[i]->type;
            if(type == 'i') {
                printf("%d ", codeobj[cnt].co_varname->stack[i]->value); }
            if(type == 't') {
                printf("%s ", codeobj[cnt].co_varname->stack[i]->string);} 
        }
    } else {
        printf(" ");
    } printf(")\n");
*/

    r_byte(fp);
    codeobj[cnt].freevarsize = (int) r_value(4, fp); 

    if(codeobj[cnt].freevarsize){
        codeobj[cnt].co_freevar = create_new_datastack(codeobj[cnt].freevarsize);
        for(i = 0; i < codeobj[cnt].freevarsize; i++) {
            type = r_byte(fp);
            if(type == 'i') {
                codeobj[cnt].co_freevar->stack[i]->type = 'i';
                codeobj[cnt].co_freevar->stack[i]->value = (int) r_value(4, fp); }
            if(type == 't') {
                stringsize = (int) r_value(4, fp);
                codeobj[cnt].co_freevar->stack[i]->type = 't';
                codeobj[cnt].co_freevar->stack[i]->string = r_bytes(stringsize, fp); }
        }
    }

 /*   printf("freevar (");
    if(codeobj[cnt].freevarsize) {
        for(i = 0; i < codeobj[cnt].freevarsize; i++) {
            type = codeobj[cnt].co_freevar->stack[i]->type;
            if(type == 'i') {
                printf("%d ", codeobj[cnt].co_freevar->stack[i]->value); }
            if(type == 't') {
                printf("%s ", codeobj[cnt].co_freevar->stack[i]->string); }
        }
    } else {
        printf(" ");
    } printf(")\n");
*/

    r_byte(fp);        
    codeobj[cnt].cellvarsize = (int) r_value(4, fp);
    if(codeobj[cnt].cellvarsize) {
        codeobj[cnt].co_cellvar = create_new_datastack(codeobj[cnt].cellvarsize);
        for(i = 0; i < codeobj[cnt].cellvarsize; i++) {
            type = r_byte(fp);
            if(type == 'i') {
                codeobj[cnt].co_cellvar->stack[i]->type = 'i';
                codeobj[cnt].co_cellvar->stack[i]->value = (int) r_value(4, fp); }
            if(type == 't') {
                codeobj[cnt].co_cellvar->stack[i]->type == 't';
                stringsize = (int) r_value(4, fp);
                codeobj[cnt].co_cellvar->stack[i]->string = r_bytes(stringsize, fp); }
        }
    }

 /*   printf("cellvar (");
    if(codeobj[cnt].cellvarsize) {
        for(i = 0; i < codeobj[cnt].cellvarsize; i++) {
            type = codeobj[cnt].co_cellvar->stack[i]->type;
            if(type == 'i') {
                printf("%d ", codeobj[cnt].co_cellvar->stack[i]->value); }
            if(type == 't') {
                printf("%s ", codeobj[cnt].co_cellvar->stack[i]->string); }
        }
    } else {
        printf(" ");
    } printf(")\n");
*/

    if((type = r_byte(fp)) == 's') {
        stringsize = (int) r_value(4, fp);
       
        codeobj[cnt].filename = r_bytes(stringsize, fp);  
  //      printf("filename: %s\n", codeobj[cnt].filename);
    }


    if((type = r_byte(fp)) == 't') {
        stringsize = (int) r_value(4, fp);
        
        codeobj[cnt].functionname = r_bytes(stringsize, fp);  
 //       printf("name : %s\n", codeobj[cnt].functionname);
    }

    codeobj[cnt].firstlineno = (int) r_value(4, fp); 
 //   printf("firstlineno : %d\n", codeobj[cnt].firstlineno);
    

    r_bytes(8, fp);



}



 












void call_execute(char *code)
{

    pyobject *temp, *temp1, *temp2, *result;
    int i, pcount, type, codecount, stringsize;
    pcount = 0;
    codecount = 0;
    blockstack = create_new_blockstack();
    while(pcount < codeobj[0].codesize ) {
        type = codeobj[0].code[pcount++];  
        switch(type) {
            case NOP :
                break;
            case LOAD_CONST :  
                type = codeobj[0].code[pcount]; 
                push(codeobj[0].co_const->stack[type]); pcount = pcount + 2;
               // r_byte(fp);  
                break;
            case STORE_NAME :
                type = codeobj[0].code[pcount]; 
                codeobj[0].co_names->stack[type]->ptr = (void*)pop();
                
                

                pcount = pcount + 2;
                break;
            case LOAD_NAME :
                type = codeobj[0].code[pcount];
                push((pyobject *)(codeobj[0].co_names->stack[type]->ptr)); 
                pcount = pcount + 2;
                break;

            case STORE_FAST :

                type = codeobj[0].code[pcount];
                
                codeobj[0].co_varname->stack[type]->ptr = (void *) pop();
    

                pcount = pcount + 2;
                break;
                
            case LOAD_FAST :
                type = codeobj[0].code[pcount];

                push((pyobject *)(codeobj[0].co_varname->stack[type]->ptr));


                pcount = pcount + 2;
                break;
            case BINARY_ADD :
                result = create_new_object();
                temp = pop();
                temp1 = pop();
                if(temp->type == 'i' && temp1->type == 'i') {
                    result->value = add(temp->value, temp1->value);
                    result->type = 'i'; }
                if(temp->type == 't' && temp1->type == 't') {
                    result->string = strcat(temp1->string, temp->string);
                    result->type = 't'; }
                push(result);
                break;
            case PRINT_ITEM :
                
                result = pop();  
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
                return pop();
                break;
*/
            case DELETE_NAME :
                type = codeobj[0].code[pcount];
                if((codeobj[0].co_names->stack[type]->ref) == 0) {
                    free(codeobj[0].co_names->stack[type]); 
                } else {
                    --(codeobj[0].co_names->stack[type]->ref); }
                pcount = pcount + 2;
                break;
            case STORE_GLOBAL :
                type = codeobj[0].code[pcount];
                codeobj[0].co_names->stack[type]->ptr = (void*) pop();
                pcount = pcount + 2;
                break;
            case LOAD_GLOBAL :
                type = codeobj[0].code[pcount];
                push((pyobject *)(codeobj[0].co_names->stack[type]->ptr));
                pcount = pcount + 2;
                break;
            case ROT_TWO :
     
                temp = pop();
                temp1 = pop();
                push(temp);
                push(temp1);
                break;
            case ROT_THREE :
               
                temp = pop();
                temp1 = pop();
                temp2 = pop();
                push(temp);
                push(temp1);
                push(temp2);
                break;
         /*   case BINARY_POWER :
           
                result = create_new_object();
                temp = pop();
                temp1 = pop();
                result->value = (int)binarypower((double)temp1->value, (double)temp->value);
                result->type = 'i';
                push(result);
                break;  */
            case BINARY_MULTIPLY :
    
                result = create_new_object();
                temp  = pop();
                temp1 = pop();
                
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
                push(result);
                break;

            case BINARY_DEVIDE :

                result = create_new_object();
                temp = pop();
                temp1 = pop();
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
                push(result);
                break;
            
            case BINARY_MODULO :
                result = create_new_object();
                temp = pop();
                temp1 = pop();
                if(temp1->type == 'i' && temp->type == 'i') {
                    result->type = 'i';
                    result->value = (temp1->value) % (temp->value); }
                push(result);
                break;
            case BINARY_FLOOR_DEVIDE :
                result = create_new_object();
                temp = pop();
                temp1 = pop();
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
                push(result);
                break;



            case BINARY_SUBTRACT :
                result = create_new_object();
                temp = pop();
                temp1 = pop();
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
                push(result);
                break;

            
            case INPLACE_ADD :
                temp = pop(); 
                temp1 = pop();
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
                push(temp1);
                break;

            case INPLACE_SUBTRACT :
                temp = pop();
                temp1 = pop();
                if(temp1->type == 'i' && temp->type == 'i') {
                    temp1->value = temp1->value - temp->value; }
                if(temp1->type == 'i' && temp->type == 'f') {
                    temp1->type = 'f';
                    temp1->fl = (float)(temp1->value) - temp->fl; }
                if(temp1->type == 'f' && temp->type == 'i') {
                    temp1->type = 'f';
                    temp1->fl = temp1->fl - (float)(temp->value); }
                if(temp1->type == 'f' && temp->type == 'f') {
                    temp1->fl = temp1->fl - temp->fl; }
                push(temp1);
                break;

            case INPLACE_MULTIPLY :
                temp = pop();
                temp1 = pop();
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
                push(temp1);
                break;
            case INPLACE_DEVIDE :
                temp = pop();
                temp1 = pop();
                if(temp1->type == 'i' && temp->type == 'i') {
                    temp1->value = temp1->value / temp->value; }
                if(temp1->type == 'i' && temp->type == 'f') {
                    temp1->type == 'f';
                    temp1->fl = (float)(temp1->value) / temp->fl; }
                if(temp1->type == 'f' && temp->type == 'i') {

                    temp1->fl = temp1->fl / (float)(temp->value); }
                if(temp1->type == 'f' && temp->type == 'f') {
                    temp1->fl = temp1->fl / temp->fl; }
                push(temp1);
                break;

            case INPLACE_MODULO :
                temp = pop();
                temp1 = pop();
                if(temp1->type == 'i' && temp->type == 'i') {
                    temp1->value = temp1->value % temp->value; }
                push(temp1);
                break;     
            case BINARY_LSHIFT :
                result = create_new_object();
                temp = pop();
                temp1 = pop();
                if(temp1->type == 'i' && temp->type == 'i') {
                    result->type = 'i';
                    result->value = temp1->value << temp->value;}
                push(result);
                break;
            case BINARY_RSHIFT :
                result = create_new_object();
                temp = pop();
                temp1 = pop();
                if(temp1->type == 'i' && temp->type == 'i') {
                    result->type = 'i';
                    result->value = temp1->value >> temp->value; }
                push(result);
                break;
            case BINARY_AND :
                result = create_new_object();
                temp = pop();
                temp1 = pop();
                if(temp1->type == 'i' && temp->type == 'i') {
                    result->type = 'i';
                    result->value = temp1->value & temp->value; }
                push(result);
                break;
            case BINARY_XOR :
                result = create_new_object();
                temp = pop();
                temp1 = pop();
                if(temp1->type == 'i' && temp->type == 'i') {
                    result->type = 'i';
                    result->value = temp1->value ^ temp->value; }
                push(result);
                break;
            case BINARY_OR :
                result = create_new_object();
                temp = pop();
                temp1 = pop();
                if(temp1->type == 'i' && temp->type == 'i') {
                    result->type = 'i';
                    result->value = temp1->value | temp->value; }
                push(result);
                break;
            case INPLACE_RSHIFT :
                temp = pop();
                temp1 = pop();
                if(temp1->type == 'i' && temp->type == 'i') {
                    temp1->value = temp1->value >> temp->value; }
                push(temp1);
                break;
            case INPLACE_LSHIFT :
                temp = pop();
                temp1 = pop();
                if(temp1->type == 'i' && temp->type == 'i') {
                    temp1->value = temp1->value << temp->value; }
                push(temp1);
                break;
            case INPLACE_AND :
                temp = pop();
                temp1 = pop();
                if(temp1->type == 'i' && temp->type == 'i') {
                    temp1->value = temp1->value & temp->value; }
                push(temp1);
                break;
            case INPLACE_OR :
                temp = pop();
                temp1 = pop();
                if(temp1->type == 'i' && temp->type == 'i') {
                    temp1->value = temp1->value | temp->value; }
                push(temp1);
                break;
            case INPLACE_XOR :
                temp = pop();
                temp1 = pop();
                if(temp1->type == 'i'  && temp->type == 'i'){
                    temp1->value = temp1->value ^ temp->value; }
                push(temp1);
                break;  
            case COMPARE_OP :
                type = codeobj[0].code[pcount];
                result = create_new_object();
                result->type = 'b';
                temp = pop();
                temp1 = pop();
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
                pcount = pcount + 2;
                push(result);
                break;
                    
            case BUILD_LIST :
                type = codeobj[0].code[pcount]; 
                result = create_new_list(type);
                push(result);
                break;
            case JUMP_IF_FALSE :
                type = codeobj[0].code[pcount]; 
                result = pop();
                if(result->type == 'b') {
                    if(result->boolvalue ==  false) {
                        pcount = type;
                    } else {
                        pcount = pcount + 2;
                    }
                }
                break;  

            case JUMP_IF_TRUE :
                type = codeobj[0].code[pcount];
                result = pop();
                if(result->type == 'b') {
                    if(result->boolvalue == true ) {
                        pcount = type;
                    } else {    
                        pcount = pcount + 2;
                    }
                }
                push(result);
                break;

            case JUMP_FORWARD :
                type = codeobj[0].code[pcount];
                pcount = pcount + type +2;
                break;
            case JUMP_ABSOLUTE :
                pcount = codeobj[0].code[pcount]; 
                break;
            case UNARY_POSETIVE :
                result = pop();  
                if(result->type == 'i'){
                    result->value = (result->value) + 1; }
                if(result->type == 'f') {
                    result->fl = (result->fl)+ 1; }
                push(result);
                break;
            case UNARY_NEGATIVE :
                result = pop();
                if(result->type == 'i') {
                    result->value = (result->value) - 1; }
                if(result->type == 'f') {
                    result->fl = (result->fl) - 1; }
                push(result);
                break;
            case UNARY_NOT :
                result = pop();
                temp = create_new_object();
                
                if(result->type == 'i') {
                    temp->type = 'i';    
                    temp->value = !(result->value); }    
                if(result->type == 'f') {
                    temp->type == 'f';
                    temp->fl = !(result->fl); }
                push(temp);
                break;

            case UNARY_INVERT :
                result = pop();
                if(result->type == 'i') {
                    result->value = ~(result->value);}


                push(result);
                break;
            case SETUP_LOOP :
                type = codeobj[0].code[pcount++];
                type = codeobj[0].code[pcount++]; 
                break;

            case POP_TOP :
                pop();
                break;
            case POP_BLOCK :
                pcount = pcount +1;  
                break;







                    
                           
                        
            default:   
                  
                break;















        }
    }
    

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
    b = (char *) malloc (n * sizeof(char));
    fread(b, 1, n, p);
    return b;
}

char r_byte(FILE *p)
{
    char b; 
    fread(&b, 1, 1, p);
    return b;
}





 void push(pyobject *item)
{
    if(mainstack->top < mainstack->size) {
        mainstack->stack[(mainstack->top)++] = item; 
    } else {
        printf("error: stack full, can't push\n");
    }
}
pyobject* pop(void) 
{
    if(mainstack->top > 0) {
        return mainstack->stack[--(mainstack->top)];  
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


STACK * create_new_localstack(int size)
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

pyobject * create_new_list(int size)
{
    int i;
    pyobject  *current, *head;
    head = NULL;
    current = NULL;
    for(i = 0; i < size; i++) {
        if(head == NULL) {
            head = create_new_object();
            head->type = '[';
            head->data = (void *)pop();
            head->ptr = NULL; 
        } else {
            current = create_new_object(); 
            current->type = '[';
            current->data = (void *)pop();
            current->ptr = (void*)head;
            head = current;
        }
    } 
    return  head;
}   

CODEOBJECT *create_new_codeobj(CODEOBJECT *codeobj)
{

  
    CODEOBJECT *new;
    if(codeobjcount == 0) {
        new = (CODEOBJECT *) malloc(sizeof(CODEOBJECT));
    } else {
        new = realloc(codeobj, codeobjcount + 1);
    }
    return new;
}

BLOCKSTACK * create_new_blockstack() 
{
    BLOCKSTACK * new;
    new = (BLOCKSTACK *) malloc(sizeof(BLOCKSTACK));
    new->top = 0;
    new->size = 5;
    new->stack = (char **) malloc( 5 * sizeof(char*)); 
    return new;
}

void pushblock(char * code)
{
    if(blockstack->top < blockstack->size) {
        blockstack->stack[(blockstack->top)++] = code;
    } else {
        printf("error: blockstack full, can't push \n");
    }
}

char * popblock(void)
{
    if(blockstack->top > 0) {
        return blockstack->stack[--(blockstack->top)];
    } else {
        printf("error: blockstack empty \n");
    }
}

        


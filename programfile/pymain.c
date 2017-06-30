  /* main function of pyvm.....
     contain two major functions... 
     read_file and call_execute...
     both are recursively call.  */

#include "opcod.h"		/* opcode header */
#include "pyvm.h"		/* virtual machine header */
#include <string.h>
#include <math.h>
#define SIZEOF_LONG 8		/* size of long */
#define bool int
#define true 1
#define false 0

/* pyobject  */
typedef struct pyobject_t {
    int type;
    bool boolvalue;
    void *data;
    int value;
    char *string;
    void *ptr;
    float fl;
    int ref;
} pyobject;

/* data and execution stack */
typedef struct STACK_t {
    int top;
    int size;
    pyobject **stack;
} STACK;

/* allowable exec stack size is 100 */
struct call_stack {
    int top;
    int size;
    int pcount;
    int cnt;
    pyobject **stack;
} mainstack[100];

/* codeobject */
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

static int cnt;
static CODEOBJECT *codeobj;
void call_execute(char *code);
void read_file(FILE * fp);
CODEOBJECT *create_new_codeobj(CODEOBJECT * codeobj);
static int codeobjcount;
static int newcnt;
static int stackcount;
STACK *create_new_datastack(int size);
void pushback(void);
void push(pyobject * item);
pyobject *pop(void);
void create_new_localstack(int size);
pyobject *create_new_object();
pyobject *create_new_list(int size);

int main(int argc, char **argv)
{
    FILE *fp;
    codeobjcount = 0;
    stackcount = 0;
    cnt = -1;
    int type;
    int magic;
    /* input arguments checking */
    if ((argc < 2) || (argc > 2)) {
	perror("usage: enter file name, or too many arguments");
	exit(1);
    }
    printf
	("****************************Python 2.7.12 ******************************\n\n");
    fp = fopen(argv[1], "rb");
    if (fp == NULL) {
	printf("can't open file %s\n", argv[1]);
	exit(1);
    }
    magic = (int) r_value(4, fp);
    if (magic != 168686339) {
	printf("version not support\n");
	exit(1);
    }
    /* starting of first codeobject */
    while ((type = r_byte(fp)) != 'c') {
	;
    }
    /* function for recursively read codeobjects and store into datablock */
    read_file(fp);
    printf
	("Executing %s............................................................\n",
	 codeobj[cnt].filename);
    create_new_localstack(codeobj[cnt].stacksize);
    /* function for recursively execute all codeobjects  */
    call_execute(codeobj[cnt].code);
    free(mainstack[stackcount - 1].stack);
    return 0;
}

/* function for recursively read all codeobjects and store into datablock */
void read_file(FILE * fp)
{
    static int type, stringsize, i, newi;
    /* create new codeobject */
    if ((codeobj = create_new_codeobj(codeobj)) == NULL) {
	printf("alloc error at 146");
    }
    ++codeobjcount;
    ++cnt;
    /*  get argcount */
    codeobj[cnt].argcount = (int) r_value(4, fp);
//printf(argcount = %d\n", codeobj[cnt].argcount);
    /* get nlocals */
    codeobj[cnt].nlocals = (int) r_value(4, fp);
    // printf ("nlocals = %d\n", codeobj[cnt].nlocals);
    /* get stacksize */
    codeobj[cnt].stacksize = (int) r_value(4, fp);
    //printf ("stacksize =  %d\n", codeobj[cnt].stacksize);
    /* check for starting of bytecode string */
    while ((type = r_byte(fp)) != 's') {
	;
    }
    /* get codesize */
    codeobj[cnt].codesize = (int) r_value(4, fp);
    // printf("codesize = %d\n", codeobj[cnt].codesize);
    codeobj[cnt].code = r_bytes(codeobj[cnt].codesize, fp);
    /* for (i = 0; i < codeobj[cnt].codesize; i++)
       {
       printf ("%d\n", codeobj[cnt].code[i]);
       }
       printf ("###\n");   */
    type = r_byte(fp);
    /* get co_const size */
    codeobj[cnt].constsize = r_value(4, fp);
    /* create new datablock for hold co_const */
    if ((codeobj[cnt].co_const =
	 create_new_datastack(codeobj[cnt].constsize)) == NULL) {
	printf("error: stack alloc @ 90\n");
    }
    if (codeobj[cnt].co_const == NULL) {
	perror("consts alloc error");
	exit(1);
    }
    /* get all conts on codeobject */
    for (i = 0; i < codeobj[cnt].constsize; i++) {
	type = r_byte(fp);
	if (type == 'i') {
	    codeobj[cnt].co_const->stack[i]->type = 'i';
	    codeobj[cnt].co_const->stack[i]->value = r_value(4, fp);
	}
	if (type == 'N') {
	    codeobj[cnt].co_const->stack[i]->type = 'N';
	    codeobj[cnt].co_const->stack[i]->value = 0;
	}
	if (type == 't' || type == 's') {
	    stringsize = (int) r_value(4, fp);
	    codeobj[cnt].co_const->stack[i]->type = 't';
	    codeobj[cnt].co_const->stack[i]->string =
		r_bytes(stringsize, fp);
	}
	if (type == 'c') {
	    codeobj[cnt].co_const->stack[i]->type = 'c';
	    codeobj[cnt].co_const->stack[i]->value = codeobjcount;
	    newcnt = cnt;
	    newi = i;
	    read_file(fp);
	    cnt = newcnt;
	    i = newi;
	}
	if (type == 'f') {
	    codeobj[cnt].co_const->stack[i]->type = 'f';
	    codeobj[cnt].co_const->stack[i]->fl = r_value(4, fp);
	}
    }
    /* print all consts */
/*
  printf ("consts (");
  if (codeobj[cnt].constsize)
    {
      for (i = 0; i < codeobj[cnt].constsize; i++)
	{
	  if (codeobj[cnt].co_const->stack[i]->type == 'i')
	    {
	      printf ("%d ", codeobj[cnt].co_const->stack[i]->value);
	    }
	  if (codeobj[cnt].co_const->stack[i]->type == 't')
	    {
	      printf ("%s ", codeobj[cnt].co_const->stack[i]->string);
	    }
	  if (codeobj[cnt].co_const->stack[i]->type == 'f')
	    {
	      printf ("%f ", codeobj[cnt].co_const->stack[i]->fl);
	    }
	  if (codeobj[cnt].co_const->stack[i]->type == 'N')
	    {
	      printf ("%d ", codeobj[cnt].co_const->stack[i]->value);
	    }
	  if (codeobj[cnt].co_const->stack[i]->type == 'c')
	    {
	      printf ("%d ", codeobj[cnt].co_const->stack[i]->value);
	    }
      if( codeobj[cnt].co_const->stack[i]->type == 'f')
        {
          printf("%f ", codeobj[cnt].co_const->stack[i]->fl);
        }
	}
    }
  else
    {
      printf (" ");
    }
  printf (")\n");
*/
    type = r_byte(fp);
    if (type == '(') {
	/* get namesize */
	codeobj[cnt].namesize = (int) r_value(4, fp);
	/* create new datablock for hold names */
	if ((codeobj[cnt].co_names =
	     create_new_datastack(codeobj[cnt].namesize)) == NULL) {
	    printf(" alloc error @163\n");
	}
    }
    /* get all names onto codeobject */
    for (i = 0; i < codeobj[cnt].namesize; i++) {
	type = r_byte(fp);
	if (type == 't') {
	    stringsize = (int) r_value(4, fp);
	    codeobj[cnt].co_names->stack[i]->type = 't';
	    codeobj[cnt].co_names->stack[i]->string =
		r_bytes(stringsize, fp);
	}
	if (type == 'i') {
	    codeobj[cnt].co_names->stack[i]->type = 'i';
	    codeobj[cnt].co_names->stack[i]->value = (int) r_value(4, fp);
	}
	if (type == 'R') {
	    codeobj[cnt].co_names->stack[i]->type = 'R';
	    codeobj[cnt].co_names->stack[i]->value = (int) r_value(4, fp);
	}
    }
    /* print names */
/*
  printf ("names(");
  if (codeobj[cnt].namesize)
    {
      for (i = 0; i < codeobj[cnt].namesize; i++)
	{
	  type = codeobj[cnt].co_names->stack[i]->type;
	  if (type == 't')
	    {
	      printf ("%s ", codeobj[cnt].co_names->stack[i]->string);
	    }
	  if (type == 'i')
	    {
	      printf ("%d ", codeobj[cnt].co_names->stack[i]->value);
	    }
	  if (type == 'R')
	    {
	      printf ("%d ", codeobj[cnt].co_names->stack[i]->value);
	    }
	}
    }
  else
    {
      printf (" ");
    }
  printf (")\n");
*/
    r_byte(fp);
    /* get varnamesize */
    codeobj[cnt].varnamesize = (int) r_value(4, fp);
    /* create new datablock for hold varname onto codeobject */
    if (codeobj[cnt].varnamesize) {
	codeobj[cnt].co_varname =
	    create_new_datastack(codeobj[cnt].varnamesize);
	for (i = 0; i < codeobj[cnt].varnamesize; i++) {
	    type = r_byte(fp);
	    if (type == 'i') {
		codeobj[cnt].co_varname->stack[i]->type = 'i';
		codeobj[cnt].co_varname->stack[i]->value =
		    (int) r_value(4, fp);
	    }
	    if (type == 't') {
		codeobj[cnt].co_varname->stack[i]->type = 't';
		stringsize = (int) r_value(4, fp);
		codeobj[cnt].co_varname->stack[i]->string =
		    r_bytes(stringsize, fp);
	    }
	}
    }
    /* print varnames */
/*
  printf ("varname (");
  if (codeobj[cnt].varnamesize)
    {
      for (i = 0; i < codeobj[cnt].varnamesize; i++)
	{
	  type = codeobj[cnt].co_varname->stack[i]->type;
	  if (type == 'i')
	    {
	      printf ("%d ", codeobj[cnt].co_varname->stack[i]->value);
	    }
	  if (type == 't')
	    {
	      printf ("%s ", codeobj[cnt].co_varname->stack[i]->string);
	    }
	}
    }
  else
    {
      printf (" ");
    }
  printf (")\n");
*/
    r_byte(fp);
    /* get free varsize */
    codeobj[cnt].freevarsize = (int) r_value(4, fp);
    /* create newdatablock for hold freevar on codeobjetc */
    if (codeobj[cnt].freevarsize) {
	codeobj[cnt].co_freevar =
	    create_new_datastack(codeobj[cnt].freevarsize);
	for (i = 0; i < codeobj[cnt].freevarsize; i++) {
	    type = r_byte(fp);
	    if (type == 'i') {
		codeobj[cnt].co_freevar->stack[i]->type = 'i';
		codeobj[cnt].co_freevar->stack[i]->value =
		    (int) r_value(4, fp);
	    }
	    if (type == 't') {
		stringsize = (int) r_value(4, fp);
		codeobj[cnt].co_freevar->stack[i]->type = 't';
		codeobj[cnt].co_freevar->stack[i]->string =
		    r_bytes(stringsize, fp);
	    }
	}
    }
    /* print freevar */
/*
  printf ("freevar (");
  if (codeobj[cnt].freevarsize)
    {
      for (i = 0; i < codeobj[cnt].freevarsize; i++)
	{
	  type = codeobj[cnt].co_freevar->stack[i]->type;
	  if (type == 'i')
	    {
	      printf ("%d ", codeobj[cnt].co_freevar->stack[i]->value);
	    }
	  if (type == 't')
	    {
	      printf ("%s ", codeobj[cnt].co_freevar->stack[i]->string);
	    }
	}
    }
  else
    {
      printf (" ");
    }
  printf (")\n");
*/
    r_byte(fp);
    /* get cellvar size */
    codeobj[cnt].cellvarsize = (int) r_value(4, fp);
    /* create new datablock for hold cellvar on codeobject */
    if (codeobj[cnt].cellvarsize) {
	codeobj[cnt].co_cellvar =
	    create_new_datastack(codeobj[cnt].cellvarsize);
	for (i = 0; i < codeobj[cnt].cellvarsize; i++) {
	    type = r_byte(fp);
	    if (type == 'i') {
		codeobj[cnt].co_cellvar->stack[i]->type = 'i';
		codeobj[cnt].co_cellvar->stack[i]->value =
		    (int) r_value(4, fp);
	    }
	    if (type == 't') {
		codeobj[cnt].co_cellvar->stack[i]->type == 't';
		stringsize = (int) r_value(4, fp);
		codeobj[cnt].co_cellvar->stack[i]->string =
		    r_bytes(stringsize, fp);
	    }
	}
    }
    /* print cellvar */
/*
  printf ("cellvar (");
  if (codeobj[cnt].cellvarsize)
    {
      for (i = 0; i < codeobj[cnt].cellvarsize; i++)
	{
	  type = codeobj[cnt].co_cellvar->stack[i]->type;
	  if (type == 'i')
	    {
	      printf ("%d ", codeobj[cnt].co_cellvar->stack[i]->value);
	    }
	  if (type == 't')
	    {
	      printf ("%s ", codeobj[cnt].co_cellvar->stack[i]->string);
	    }
	}
    }
  else
    {
      printf (" ");
    }
  printf (")\n");
*/
    if ((type = r_byte(fp)) == 's') {
	stringsize = (int) r_value(4, fp);
	/* filename read */
	codeobj[cnt].filename = r_bytes(stringsize, fp);
	//     printf("filename: %s\n", codeobj[cnt].filename);
    }
    type = r_byte(fp);
    if (type == 't') {
	stringsize = (int) r_value(4, fp);
	/* functionname read */
	codeobj[cnt].functionname = r_bytes(stringsize, fp);
	//   printf("functionname : %s\n", codeobj[cnt].functionname);
    }
    if (type == 'R') {
	codeobj[cnt].functionname = r_bytes(4, fp);
	// printf("functionname : %s\n", codeobj[cnt].functionname);
    }
    /* read firstline number */
    codeobj[cnt].firstlineno = (int) r_value(4, fp);
    // printf("firstlineno : %d\n", codeobj[cnt].firstlineno);
    type = r_byte(fp);
    stringsize = (int) r_value(4, fp);
    r_bytes(stringsize, fp);
}

/* recursively execute all codeobjects */
void call_execute(char *code)
{
    pyobject *temp, *temp1, *temp2, *result, *arg1, *arg2, *arg3, *arg4;
    int i, pcount, type, codecount, stringsize;
    pcount = 0;
    /* loop over all instructions */
    while (pcount < codeobj[cnt].codesize) {
	type = codeobj[cnt].code[pcount++];
	switch (type) {
	case NOP:
	    break;
	case LOAD_CONST:	/* load const to stack */
	    type = codeobj[cnt].code[pcount];
	    push(codeobj[cnt].co_const->stack[type]);
	    pcount = pcount + 2;
	    break;
	case STORE_NAME:	/* store ref of object to a name object */
	    type = codeobj[cnt].code[pcount];
	    codeobj[cnt].co_names->stack[type]->ptr = (void *) pop();
	    pcount = pcount + 2;
	    break;
	case LOAD_NAME:	/* load name contain ref of object to stack */
	    type = codeobj[cnt].code[pcount];
	    push((pyobject *) (codeobj[cnt].co_names->stack[type]->ptr));
	    pcount = pcount + 2;
	    break;
	case STORE_FAST:	/* store ref of object to a varname objcet */
	    type = codeobj[cnt].code[pcount];
	    codeobj[cnt].co_varname->stack[type]->ptr = (void *) pop();
	    pcount = pcount + 2;
	    break;
	case LOAD_FAST:	/* load varname contain ref of object to stack */
	    type = codeobj[cnt].code[pcount];
	    push((pyobject *) (codeobj[cnt].co_varname->stack[type]->ptr));
	    pcount = pcount + 2;
	    break;
	case BINARY_ADD:	/* add object and push onto stack */
	    result = create_new_object();
	    temp = pop();
	    temp1 = pop();
	    if (temp->type == 'i' && temp1->type == 'i') {
		result->value = add(temp->value, temp1->value);
		result->type = 'i';
	    }
	    if (temp->type == 't' && temp1->type == 't') {
		result->string = strcat(temp1->string, temp->string);
		result->type = 't';
	    }
	    push(result);
	    break;
	case PRINT_ITEM:	/* pop object from stack and print it on stdout */
	    result = pop();
	    if (result->type == 'i') {
		printf("%d", result->value);
	    }
	    if (result->type == 't' || type == 's') {
		printf("%s", result->string);
	    }
	    if (result->type == 'f') {
		printf("%f", result->fl);
	    }
	    if (result->type == 'b') {
		if (result->boolvalue == true) {
		    printf("true");
		} else {
		    printf("false");
		}
	    }
	    if (result->type == '[') {
		temp = result;
		printf("[");
		while (result != NULL) {
		    temp = (pyobject *) (result->data);
		    if (temp->type == 'i') {
			printf("%d ", temp->value);
		    }
		    if (temp->type == 'f') {
			printf("%f ", temp->fl);
		    }
		    if (temp->type == 't' || temp->type == 's') {
			printf("%s ", temp->string);
		    }
		    result = (pyobject *) (result->ptr);
		}
		printf("]\n");
	    }
	    break;
	case PRINT_NEWLINE:	/* print newline on stdout */
	    printf("\n");
	    break;
	case RETURN_VALUE:	/* pop object from current stack and push onto previous stack */
	    if (stackcount > 1) {
		pushback();
		free(mainstack[--stackcount].stack);	/*clear the current stack and operation back on to previous stack */
		cnt = mainstack[stackcount - 1].cnt;
		pcount = mainstack[stackcount - 1].pcount + 2;
	    }
	    break;
	case DELETE_NAME:	/* delete name from name list */
	    type = codeobj[cnt].code[pcount];
	    if ((codeobj[cnt].co_names->stack[type]->ref) == 0) {
		free(codeobj[cnt].co_names->stack[type]);
	    } else {
		--(codeobj[cnt].co_names->stack[type]->ref);
	    }
	    pcount = pcount + 2;
	    break;
	case STORE_GLOBAL:	/* store object ref to name as global */
	    type = codeobj[cnt].code[pcount];
	    codeobj[cnt].co_names->stack[type]->ptr = (void *) pop();
	    pcount = pcount + 2;
	    break;
	case LOAD_GLOBAL:	/* push global object onto stack */
	    type = codeobj[cnt].code[pcount];
	    push((pyobject *) (codeobj[cnt].co_names->stack[type]->ptr));
	    pcount = pcount + 2;
	    break;
	case ROT_TWO:		/* reposition top and second top object */
	    temp = pop();
	    temp1 = pop();
	    push(temp);
	    push(temp1);
	    break;
	case ROT_THREE:	/* repostion top on second top and first top on top and second top on first top */
	    temp = pop();
	    temp1 = pop();
	    temp2 = pop();
	    push(temp);
	    push(temp1);
	    push(temp2);
	    break;
	case BINARY_MULTIPLY:	/* multiply two object and push onto stack */
	    result = create_new_object();
	    temp = pop();
	    temp1 = pop();
	    if (temp->type == 'i' && temp1->type == 'i') {
		result->value = (temp1->value) * (temp->value);
		result->type = 'i';
	    }
	    if (temp1->type == 'i' && temp->type == 'f') {
		result->type = 'f';
		result->fl = (temp1->value) * (temp->fl);
	    }
	    if (temp1->type == 'f' && temp->type == 'i') {
		result->type = 'f';
		result->fl = (temp1->fl) * (temp->value);
	    }
	    if (temp1->type == 'f' && temp->type == 'f') {
		result->type = 'f';
		result->fl = (temp1->fl) * (temp->fl);
	    }
	    if (temp1->type == 't' && temp->type == 'i') {
		stringsize = strlen(temp1->string);
		stringsize = stringsize * (temp->value);
		if ((result->string =
		     (char *) malloc(sizeof(char) * stringsize)) == NULL) {
		    printf("error: realloc @293\n");
		}
		for (i = 0; i < (temp->value); i++) {
		    strcat(result->string, temp1->string);
		}
		result->type = 't';
	    }
	    push(result);
	    break;
	case BINARY_DEVIDE:	/* devide tow object and push onto stack */
	    result = create_new_object();
	    temp = pop();
	    temp1 = pop();
	    if (temp1->type == 'i' && temp->type == 'i') {
		result->type = 'i';
		result->value = (temp1->value) / (temp->value);
	    }
	    if (temp1->type == 'i' && temp->type == 'f') {
		result->type = 'f';
		result->fl = (temp1->value) / (temp->fl);
	    }
	    if (temp1->type == 'f' && temp->type == 'i') {
		result->type = 'f';
		result->fl = (temp1->fl) / (temp->value);
	    }
	    if (temp1->type == 'f' && temp->type == 'f') {
		result->type = 'f';
		result->fl = (temp1->fl) / (temp->fl);
	    }
	    push(result);
	    break;
	case BINARY_MODULO:	/* find moduloous of two object and result push onto stack  */
	    result = create_new_object();
	    temp = pop();
	    temp1 = pop();
	    if (temp1->type == 'i' && temp->type == 'i') {
		result->type = 'i';
		result->value = (temp1->value) % (temp->value);
	    }
	    push(result);
	    break;
	case BINARY_FLOOR_DEVIDE:	/* binary floor devide two object and result push onto stack */
	    result = create_new_object();
	    temp = pop();
	    temp1 = pop();
	    if (temp1->type == 'i' && temp->type == 'i') {
		result->type = 'i';
		result->value = (temp1->value) / (temp->value);
	    }
	    if (temp1->type == 'i' && temp->type == 'f') {
		result->type = 'i';
		result->value = ((temp1->value) / (int) (temp->fl));
	    }
	    if (temp1->type == 'f' && temp->type == 'i') {
		result->type = 'i';
		result->value = ((int) (temp1->fl) / (temp->value));
	    }
	    if (temp1->type == 'f' && temp->type == 'f') {
		result->type = 'i';
		result->value = ((int) (temp1->fl) / (int) (temp->fl));
	    }
	    push(result);
	    break;
	case BINARY_SUBTRACT:	/* subtract two object and result push onto stack */
	    result = create_new_object();
	    temp = pop();
	    temp1 = pop();
	    if (temp1->type == 'i' && temp->type == 'i') {
		result->type = 'i';
		result->value = (temp1->value) - (temp->value);
	    }
	    if (temp1->type == 'i' && temp->type == 'f') {
		result->type = 'f';
		result->fl = (temp1->value) - (temp->fl);
	    }
	    if (temp1->type == 'f' && temp->type == 'i') {
		result->type = 'f';
		result->fl = (temp1->fl) - (temp->value);
	    }
	    if (temp1->type == 'f' && temp->type == 'f') {
		result->type = 'f';
		result->fl = (temp1->fl) - (temp->fl);
	    }
	    push(result);
	    break;
	case INPLACE_ADD:	/* inplace add two object and result push onto stack */
	    temp = pop();
	    temp1 = pop();
	    if (temp1->type == 'i' && temp->type == 'i') {
		temp1->value = temp1->value + temp->value;
	    }
	    if (temp1->type == 'i' && temp->type == 'f') {
		temp1->type = 'f';
		temp1->fl = (float) (temp1->value) + temp->fl;
	    }
	    if (temp1->type == 'f' && temp->type == 'i') {
		temp1->fl = temp1->fl + (float) (temp->value);
	    }
	    if (temp1->type == 'f' && temp->type == 'f') {
		temp1->fl = temp1->fl + temp->fl;
	    }
	    if (temp1->type == 't' && temp->type == 't') {
		temp1->string = strcat(temp1->string, temp->string);
	    }
	    push(temp1);
	    break;
	case INPLACE_SUBTRACT:	/* inplace subtract two object and result push onto stack */
	    temp = pop();
	    temp1 = pop();
	    if (temp1->type == 'i' && temp->type == 'i') {
		temp1->value = temp1->value - temp->value;
	    }
	    if (temp1->type == 'i' && temp->type == 'f') {
		temp1->type = 'f';
		temp1->fl = (float) (temp1->value) - temp->fl;
	    }
	    if (temp1->type == 'f' && temp->type == 'i') {
		temp1->type = 'f';
		temp1->fl = temp1->fl - (float) (temp->value);
	    }
	    if (temp1->type == 'f' && temp->type == 'f') {
		temp1->fl = temp1->fl - temp->fl;
	    }
	    push(temp1);
	    break;
	case INPLACE_MULTIPLY:	/* inplace multiply two object and result push onto stack */
	    temp = pop();
	    temp1 = pop();
	    if (temp1->type == 'i' && temp->type == 'i') {
		temp1->type = 'i';
		temp1->value = temp1->value * temp->value;
	    }
	    if (temp1->type == 'i' && temp->type == 'f') {
		temp1->type = 'f';
		temp1->fl = (float) (temp1->value) * temp->fl;
	    }
	    if (temp1->type == 'f' && temp->type == 'i') {
		temp1->type == 'f';
		temp1->fl = temp1->fl * (float) (temp->value);
	    }
	    if (temp1->type == 'f' && temp->type == 'f') {
		temp1->fl = temp1->fl * temp->fl;
	    }
	    push(temp1);
	    break;
	case INPLACE_DEVIDE:	/* inplace devide two object and result push onto stack */
	    temp = pop();
	    temp1 = pop();
	    if (temp1->type == 'i' && temp->type == 'i') {
		temp1->value = temp1->value / temp->value;
	    }
	    if (temp1->type == 'i' && temp->type == 'f') {
		temp1->type == 'f';
		temp1->fl = (float) (temp1->value) / temp->fl;
	    }
	    if (temp1->type == 'f' && temp->type == 'i') {
		temp1->fl = temp1->fl / (float) (temp->value);
	    }
	    if (temp1->type == 'f' && temp->type == 'f') {
		temp1->fl = temp1->fl / temp->fl;
	    }
	    push(temp1);
	    break;
	case INPLACE_MODULO:	/* find inplace modulous of two objects and result push onto stack */
	    temp = pop();
	    temp1 = pop();
	    if (temp1->type == 'i' && temp->type == 'i') {
		temp1->value = temp1->value % temp->value;
	    }
	    push(temp1);
	    break;
	case BINARY_LSHIFT:	/* left shift int object */
	    result = create_new_object();
	    temp = pop();
	    temp1 = pop();
	    if (temp1->type == 'i' && temp->type == 'i') {
		result->type = 'i';
		result->value = temp1->value << temp->value;
	    }
	    push(result);
	    break;
	case BINARY_RSHIFT:	/* right an integer object  */
	    result = create_new_object();
	    temp = pop();
	    temp1 = pop();
	    if (temp1->type == 'i' && temp->type == 'i') {
		result->type = 'i';
		result->value = temp1->value >> temp->value;
	    }
	    push(result);
	    break;
	case BINARY_AND:	/* find and of two objects */
	    result = create_new_object();
	    temp = pop();
	    temp1 = pop();
	    if (temp1->type == 'i' && temp->type == 'i') {
		result->type = 'i';
		result->value = temp1->value & temp->value;
	    }
	    push(result);
	    break;
	case BINARY_XOR:	/* find xor value of two objects and result push onto stack */
	    result = create_new_object();
	    temp = pop();
	    temp1 = pop();
	    if (temp1->type == 'i' && temp->type == 'i') {
		result->type = 'i';
		result->value = temp1->value ^ temp->value;
	    }
	    push(result);
	    break;
	case BINARY_OR:	/* find bonary or of two objects and result push onto stack */
	    result = create_new_object();
	    temp = pop();
	    temp1 = pop();
	    if (temp1->type == 'i' && temp->type == 'i') {
		result->type = 'i';
		result->value = temp1->value | temp->value;
	    }
	    push(result);
	    break;
	case INPLACE_RSHIFT:	/* inplace rshift an object */
	    temp = pop();
	    temp1 = pop();
	    if (temp1->type == 'i' && temp->type == 'i') {
		temp1->value = temp1->value >> temp->value;
	    }
	    push(temp1);
	    break;
	case INPLACE_LSHIFT:	/* inplace left shift of an object */
	    temp = pop();
	    temp1 = pop();
	    if (temp1->type == 'i' && temp->type == 'i') {
		temp1->value = temp1->value << temp->value;
	    }
	    push(temp1);
	    break;
	case INPLACE_AND:	/* findinplace and of object */
	    temp = pop();
	    temp1 = pop();
	    if (temp1->type == 'i' && temp->type == 'i') {
		temp1->value = temp1->value & temp->value;
	    }
	    push(temp1);
	    break;
	case INPLACE_OR:	/* find inplace or of objects */
	    temp = pop();
	    temp1 = pop();
	    if (temp1->type == 'i' && temp->type == 'i') {
		temp1->value = temp1->value | temp->value;
	    }
	    push(temp1);
	    break;
	case INPLACE_XOR:	/* find inplace xor of objects */
	    temp = pop();
	    temp1 = pop();
	    if (temp1->type == 'i' && temp->type == 'i') {
		temp1->value = temp1->value ^ temp->value;
	    }
	    push(temp1);
	    break;
	case COMPARE_OP:	/* basic compare operation <, <=, > , >=, ==, != and */
	    /* store result as bool type object and push result onto stack */
	    type = codeobj[cnt].code[pcount];
	    result = create_new_object();
	    result->type = 'b';
	    temp = pop();
	    temp1 = pop();
	    if (type == 0) {
		if (temp->type == 'i') {
		    if (temp1->value < temp->value) {
			result->boolvalue = true;
		    } else {
			result->boolvalue = false;
		    }
		}
		if (temp->type == 'f') {
		    if (temp1->fl < temp->fl) {
			result->boolvalue = true;
		    } else {
			result->boolvalue = false;
		    }
		}
	    }
	    if (type == 1) {
		if (temp->type == 'i') {
		    if (temp1->value <= temp->value) {
			result->boolvalue = true;
		    } else {
			result->boolvalue = false;
		    }
		}
		if (temp->type == 'f') {
		    if (temp1->fl <= temp->fl) {
			result->boolvalue = true;
		    } else {
			result->boolvalue = false;
		    }
		}
	    }
	    if (type == 2) {
		if (temp->type == 'i') {
		    if (temp1->value == temp->value) {
			result->boolvalue = true;
		    } else {
			result->boolvalue = false;
		    }
		}
		if (temp->type == 'f') {
		    if (temp1->fl == temp->fl) {
			result->boolvalue = true;
		    } else {
			result->boolvalue = false;
		    }
		}
		if (temp->type == 't' || temp->type == 's') {
		    if (strcmp(temp1->string, temp->string) == 0) {
			result->boolvalue = true;
		    } else {
			result->boolvalue = false;
		    }
		}
	    }
	    if (type == 3) {
		if (temp->type == 'i') {
		    if (temp1->value != temp->value) {
			result->boolvalue = true;
		    } else {
			result->boolvalue = false;
		    }
		}
		if (temp->type == 'f') {
		    if (temp1->fl != temp->fl) {
			result->boolvalue = true;
		    } else {
			result->boolvalue = false;
		    }
		}
		if (temp->type == 't' || temp->type == 's') {
		    if (strcmp(temp1->string, temp->string) != 0) {
			result->boolvalue = true;
		    } else {
			result->boolvalue = false;
		    }
		}
	    }
	    if (type == 4) {
		if (temp->type == 'i') {
		    if (temp1->value > temp->value) {
			result->boolvalue = true;
		    } else {
			result->boolvalue = false;
		    }
		}
		if (temp->type == 'f') {
		    if (temp1->fl > temp->fl) {
			result->boolvalue = true;
		    } else {
			result->boolvalue = false;
		    }
		}
	    }
	    if (type == 5) {
		if (temp->type == 'i') {
		    if (temp1->value >= temp->value) {
			result->boolvalue = true;
		    } else {
			result->boolvalue = false;
		    }
		}
		if (temp->type == 'f') {
		    if (temp1->fl >= temp->fl) {
			result->boolvalue = true;
		    } else {
			result->boolvalue = false;
		    }
		}
	    }
	    pcount = pcount + 2;
	    push(result);
	    break;
	case BUILD_LIST:	/* pop required objects from stack and make a list and push onto stack */
	    type = codeobj[cnt].code[pcount];
	    result = create_new_list(type);
	    push(result);
	    break;
	case JUMP_IF_FALSE:	/* jump to instruction if object value is false */
	    type = codeobj[cnt].code[pcount];
	    result = pop();
	    if (result->type == 'b') {
		if (result->boolvalue == false) {
		    pcount = type;
		} else {
		    pcount = pcount + 2;
		}
	    }
	    if (result->type == 'i') {
		if (result->value == 0) {
		    pcount = type;
		} else {
		    pcount = pcount + 2;
		}
	    }


	    break;
	case JUMP_IF_TRUE:	/* jump to instruction if object value is true */
	    type = codeobj[cnt].code[pcount];
	    result = pop();
	    if (result->type == 'b') {
		if (result->boolvalue == true) {
		    pcount = type;
		} else {
		    pcount = pcount + 2;
		}
	    }
	    push(result);
	    break;
	case JUMP_FORWARD:	/* jump forward according to oparg */
	    type = codeobj[cnt].code[pcount];
	    pcount = pcount + type + 2;
	    break;
	case JUMP_ABSOLUTE:	/* jump to a particular intruction */
	    type = codeobj[cnt].code[pcount];
	    pcount = type;
	    break;
	case UNARY_POSETIVE:	/* make object +ve */
	    result = pop();
	    if (result->type == 'i') {
		result->value = (result->value) + 1;
	    }
	    if (result->type == 'f') {
		result->fl = (result->fl) + 1;
	    }
	    push(result);
	    break;
	case UNARY_NEGATIVE:	/* make object -ve */
	    result = pop();
	    if (result->type == 'i') {
		result->value = (result->value) - 1;
	    }
	    if (result->type == 'f') {
		result->fl = (result->fl) - 1;
	    }
	    push(result);
	    break;
	case UNARY_NOT:	/* make object to logic not */
	    result = pop();
	    temp = create_new_object();
	    if (result->type == 'i') {
		temp->type = 'i';
		temp->value = !(result->value);
	    }
	    if (result->type == 'f') {
		temp->type == 'f';
		temp->fl = !(result->fl);
	    }
	    push(temp);
	    break;
	case UNARY_INVERT:	/* invert object */
	    result = pop();
	    if (result->type == 'i') {
		result->value = ~(result->value);
	    }
	    push(result);
	    break;
	case SETUP_LOOP:
	    pcount = pcount + 2;
	    break;
	case POP_TOP:		/* pop out top most object from stack */
	    pop();
	    break;
	case POP_BLOCK:
	    break;
	case MAKE_FUNCTION:	/* make function */
	    result = pop();
	    if (result->type == 'c') {
		result->string = codeobj[result->value].code;
		push(result);
	    }
	    pcount = pcount + 2;
	    break;
	case CALL_FUNCTION:	/* call a particularfunction by oparg */
	    type = codeobj[cnt].code[pcount];
	    if (type == 1) {	/* store input arg for function on varname */
		arg1 = pop();
		codeobj[1].co_varname->stack[0]->ptr = (void *) arg1;
	    }
	    if (type == 2) {
		arg2 = pop();
		arg1 = pop();
		codeobj[1].co_varname->stack[0]->ptr = (void *) arg1;
		codeobj[1].co_varname->stack[1]->ptr = (void *) arg2;
	    }
	    if (type == 3) {
		arg3 = pop();
		arg2 = pop();
		arg1 = pop();
		codeobj[1].co_varname->stack[0]->ptr = (void *) arg1;
		codeobj[1].co_varname->stack[1]->ptr = (void *) arg2;
		codeobj[1].co_varname->stack[2]->ptr = (void *) arg3;
	    }
	    if (type == 4) {
		arg4 = pop();
		arg3 = pop();
		arg2 = pop();
		arg1 = pop();
		codeobj[1].co_varname->stack[0]->ptr = (void *) arg1;
		codeobj[1].co_varname->stack[1]->ptr = (void *) arg2;
		codeobj[1].co_varname->stack[2]->ptr = (void *) arg3;
		codeobj[1].co_varname->stack[3]->ptr = (void *) arg4;
	    }
	    pop();
	    mainstack[stackcount - 1].pcount = pcount;	/* store state of call function */
	    mainstack[stackcount - 1].cnt = cnt;
	    cnt = 1;
	    create_new_localstack(codeobj[cnt].stacksize);
	    mainstack[stackcount - 1].pcount = 0;
	    mainstack[stackcount - 1].cnt = cnt;
	    call_execute(codeobj[cnt].code);
	    /* calling the function to be exec */
	    break;
	default:
	    pcount = pcount + 2;
	    break;
	}
    }
}

/* read long value */
long r_value(int n, FILE * p)
{				/* recovering original value fron memory and store as long and return */
    long x = -1;
    const unsigned char *buf;
    int nr;
    if ((buf = (const unsigned char *) r_bytes(n, p)) != NULL) {
	x = buf[0];
	x |= (long) buf[1] << 8;
	x |= (long) buf[2] << 16;
	x |= (long) buf[3] << 24;
#if SIZEOF_LONG > 4
	x |= -(x & 0x80000000L);	/*only for 64 bit */
#endif
    }
    return x;
}

/* read string of bytes */
static char *r_bytes(int n, FILE * p)
{				/* read bytes */
    static char *b;
    b = (char *) malloc(n * sizeof(char));
    fread(b, 1, n, p);
    return b;
}

/* read one byte */
char r_byte(FILE * p)
{				/* read single byte */
    char b;
    fread(&b, 1, 1, p);
    return b;
}

/* push pyobject onto stack */
void push(pyobject * item)
{
    if (mainstack[stackcount - 1].top < mainstack[stackcount - 1].size) {
	mainstack[stackcount -
		  1].stack[(mainstack[stackcount - 1].top)++] = item;
    } else {
	// printf ("error: stack full, can't push\n");
    }
}

/*pop pyobject out of stack */
pyobject *pop(void)
{
    if (mainstack[stackcount - 1].top > 0) {
	return mainstack[stackcount -
			 1].stack[--(mainstack[stackcount - 1].top)];
    } else {
	// printf ("error: stack empty\n");
    }
}

/* create new datastack for hold data associated with codeobject */
STACK *create_new_datastack(int size)
{
    int i;
    STACK *new = (STACK *) malloc(sizeof(STACK));
    new->top = 0;
    new->size = size;
    new->stack = (pyobject **) malloc(size * sizeof(pyobject *));
    for (i = 0; i < size; i++) {
	if ((new->stack[i] =
	     (pyobject *) malloc(sizeof(pyobject))) == NULL) {
	    printf("error: stack alloc @ 297\n");
	}
    }
    return new;
}

/* create new local stack for hold object reference for execution */
void create_new_localstack(int size)
{
    if (stackcount > 99) {
	printf("maximum recursion depth exceeded\n");
	exit(1);
    }
    if ((mainstack[stackcount].stack =
	 (pyobject **) malloc(size * sizeof(pyobject *))) == NULL) {
	printf("malloc error @ 1461\n");
	exit(1);
    }
    mainstack[stackcount].top = 0;
    mainstack[stackcount].size = size;
    ++stackcount;
}

/* create new  data object */
pyobject *create_new_object()
{
    pyobject *new;
    new = malloc(sizeof(pyobject));
    return new;
}

/* create new list datastructure */
pyobject *create_new_list(int size)
{
    int i;
    pyobject *current, *head;
    head = NULL;
    current = NULL;
    for (i = 0; i < size; i++) {
	if (head == NULL) {
	    head = create_new_object();
	    head->type = '[';
	    head->data = (void *) pop();
	    head->ptr = NULL;
	} else {
	    current = create_new_object();
	    current->type = '[';
	    current->data = (void *) pop();
	    current->ptr = (void *) head;
	    head = current;
	}
    }
    return head;
}

/* create new codeobject stack for each codeobject data */
CODEOBJECT *create_new_codeobj(CODEOBJECT * codeobj)
{
    CODEOBJECT *new;
    if (codeobjcount == 0) {
	if ((new = (CODEOBJECT *) malloc(sizeof(CODEOBJECT))) == NULL) {
	    printf("alooc error @ 1183\n");
	}
    } else {
	if ((new = realloc(codeobj, codeobjcount + 1)) == NULL) {
	    printf("alloc error @ 1186\n");
	}
    }
    return new;
}

/* pop object from called function stack and push onto callee stack  */
void pushback(void)
{
    pyobject *result;
    if (mainstack[stackcount - 1].top > 0) {
	result =
	    mainstack[stackcount -
		      1].stack[--(mainstack[stackcount - 1].top)];
    }
    if (mainstack[stackcount - 2].top < mainstack[stackcount - 2].size) {
	mainstack[stackcount -
		  2].stack[(mainstack[stackcount - 2].top)++] = result;
    }
}

/*  add two int value */
int add(int x, int y)
{
    return x + y;
}


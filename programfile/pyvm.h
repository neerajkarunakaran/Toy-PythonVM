#include <stdio.h> 
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <string.h>
char r_byte(FILE * p);		/*  read a byte */
long r_value(int n, FILE * p);	/* read bytes and convert to long */
static char *r_bytes(int n, FILE * p);	/* read bytes as string */
int add(int x, int y);		/* addition function */

#ifndef __SRC_STRING__
#define __SRC_STRING__

#include <string.h>
#include <stdio.h>
#include "machine.h"
#include "stack-c.h"

void strsplit1(char **,char **,int *,int *,int *,int ,int *);
void tokens(char **,char **,char **,int *,int *,int ,int );
void code2str(char **,int , int , int *);
void convstr(char **, char **, char , int );
void stripblanks(char **,char **,int ,int );
void str2code(char **,int *,int *);
void scistrcat(char **,char **,int );
#endif
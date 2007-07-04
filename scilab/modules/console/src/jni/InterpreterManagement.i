/* InterpreterManagement.i */
/*  swig -java -package org.scilab.modules.console -outdir ../java/org/scilab/modules/console/ InterpreterManagement.i */
%module InterpreterManagement
%{
#include "../c/InterpreterManagement.h"
%}
int PutCommandInScilabQueue(char *command);
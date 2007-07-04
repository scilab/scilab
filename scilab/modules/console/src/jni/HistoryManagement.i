/* HistoryManagement.i */
/* To run this file execute the command below from current directory */
/*  swig -java -package org.scilab.modules.console -outdir ../java/org/scilab/modules/console/ HistoryManagement.i */
%module HistoryManagement
%{
#include "../c/HistoryManagement.h"
%}
char *getNextEntry(char *beg);
char *getPreviousEntry(char *beg);
void setNewSearchInHistory(int state);
char *searchBackward(char *line);
void AddHistory(char *line);
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//======================================
// test strsplit
// memory leak
//======================================
clear;
FUNCTION_TO_TEST = "strsplit";
NB_LOOPS = 1000000;
REF_4_1_2 = 0;
[BEFORE_FREE,BEFORE_TOTAL] = getmemory();

STR_SPLITED = ['strsplit splits';'a string';'into';'a vector of strings'];
STR = 'strsplit splits a string into a vector of strings';
INDICES = [15 25 30];

for i=1:NB_LOOPS 
  r = strsplit(STR,INDICES);
  clear r;
end  

[AFTER_FREE,AFTER_TOTAL] = getmemory();
BEFORE_USED = BEFORE_TOTAL - BEFORE_FREE;
AFTER_USED = AFTER_TOTAL - AFTER_FREE;
if (abs(BEFORE_USED - AFTER_USED) > REF_4_1_2 ) then
  warning(FUNCTION_TO_TEST+" MEMORY LEAK : "+string(abs(BEFORE_USED - AFTER_USED))+ ' ko' );
end  
//====================================

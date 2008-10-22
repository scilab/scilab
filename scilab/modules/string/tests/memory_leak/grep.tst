// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//======================================
// test grep
// memory leak
//======================================
clear;
FUNCTION_TO_TEST = "grep";
NB_LOOPS = 1000000;
txt=['find matches of a string in a vector of strings'
     'search position of a character string in an other string'
     'Compare Strings'];

REF_4_1_2 = 0;
[BEFORE_FREE,BEFORE_TOTAL] = getmemory();

for i=1:NB_LOOPS 
  [a,b]=grep(txt,['strings' 'Strings']);
  clear a;
  clear b;
end  

[AFTER_FREE,AFTER_TOTAL] = getmemory();
BEFORE_USED = BEFORE_TOTAL - BEFORE_FREE;
AFTER_USED = AFTER_TOTAL - AFTER_FREE;
if (abs(BEFORE_USED - AFTER_USED) > REF_4_1_2 ) then
  warning(FUNCTION_TO_TEST+" MEMORY LEAK : "+string(abs(BEFORE_USED - AFTER_USED))+ ' ko' );
end  
//====================================

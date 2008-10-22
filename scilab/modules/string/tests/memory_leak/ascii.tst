// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//======================================
// test ascii
// memory
//======================================
clear;
NB_LOOPS = 1000000;
REF_4_1_2 = 0;
[BEFORE_FREE,BEFORE_TOTAL] = getmemory();
BEFORE_USED = BEFORE_TOTAL - BEFORE_FREE;

for i=1:NB_LOOPS 
  r = ascii(ascii("scilab"));
  clear r;
end
  
[AFTER_FREE,AFTER_TOTAL] = getmemory();
AFTER_USED = AFTER_TOTAL - AFTER_FREE;
if (AFTER_USED - BEFORE_USED > REF_4_1_2 ) then disp("MEMORY LEAK"),end;
//====================================

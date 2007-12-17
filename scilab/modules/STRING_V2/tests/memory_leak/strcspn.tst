//======================================
// test strcspn
// memory leak
// A.C INRIA 2007
//======================================
clear;
FUNCTION_TO_TEST = "strcspn";
NB_LOOPS = 1000000;
REF_4_1_2 = 0;
[BEFORE_FREE,BEFORE_TOTAL] = getmemory();

STR1 = 'Scilab';
STR2 = 'The Numerical Tool for Engineers';
STR = [STR1,STR2;STR2,STR1];

for i=1:NB_LOOPS 
  r = strcspn(STR,'i');
  clear r;
end  

[AFTER_FREE,AFTER_TOTAL] = getmemory();
BEFORE_USED = BEFORE_TOTAL - BEFORE_FREE;
AFTER_USED = AFTER_TOTAL - AFTER_FREE;
if (abs(BEFORE_USED - AFTER_USED) > REF_4_1_2 ) then
  warning(FUNCTION_TO_TEST+" MEMORY LEAK : "+string(abs(BEFORE_USED - AFTER_USED))+ ' ko' );
end  
//====================================

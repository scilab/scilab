//======================================
// test strncpy
// memory leak
// A.C INRIA 2007
//======================================
clear;
FUNCTION_TO_TEST = "strncpy";
NB_LOOPS = 1000000;
REF_4_1_2 = 0;
[BEFORE_FREE,BEFORE_TOTAL] = getmemory();

str1 = "Scilab string 1";
str2 = "Scilab string 2";
STR = [str1,str2;str2,str1];

for i=1:NB_LOOPS 
  r = strncpy(STR,[1,2;3,4]);
  clear r;
end  

[AFTER_FREE,AFTER_TOTAL] = getmemory();
BEFORE_USED = BEFORE_TOTAL - BEFORE_FREE;
AFTER_USED = AFTER_TOTAL - AFTER_FREE;
if (abs(BEFORE_USED - AFTER_USED) > REF_4_1_2 ) then
  warning(FUNCTION_TO_TEST+" MEMORY LEAK : "+string(abs(BEFORE_USED - AFTER_USED))+ ' ko' );
end  
//====================================

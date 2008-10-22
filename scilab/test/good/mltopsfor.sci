function [res]=mltopsfor(symb)
//!
// Copyright INRIA
stk;
s1=stk(top-1)
s2=stk(top)
if symb+s2(2)=='**1' then s2(2)='2',end
if symb+s1(2)=='**1' then s1(2)='2',end
if symb+s2(2)=='/1' then s2(2)='2',end
if symb+s2(2)=='./1' then s2(2)='2',end
//if symb+s1(2)=='\1' then s1(2)='2',end
//if symb+s1(2)=='.\1' then s1(2)='2',end
if s2(2)=='2' then s2(1)='('+s2(1)+')',end
if s1(2)=='2' then s1(1)='('+s1(1)+')',end
res=list(s1(1)+symb+s2(1),'1')
if symb=='\' then res(2)='2',end
if symb=='.\' then res(2)='2',end
//if symb=='**' then res(2)='2',end
endfunction

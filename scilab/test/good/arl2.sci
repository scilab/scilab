function   [den,num,err]=arl2(y,den0,n,imp,all)
// steer, jpc, fd 1997 (Nov)
// Copyright INRIA
[lhs,rhs]=argn(0);
// test the system type 'c' 'd' or dt 
if rhs <= 4 then all='one';end
if rhs <= 3 then imp=0;end
if all=='all' then
  [den,num,err]=arl2_ius(y,den0,n,imp,all);
else
  [den,num,err]=arl2_ius(y,den0,n,imp);
end;
if lhs<=1 then
  den=syslin('d',num,den);
end
endfunction

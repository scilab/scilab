function [stk,txt,top]=sci_ferror()
// Copyright INRIA
txt=[]
fid=stk(top-rhs+1)
if lhs==2 then
  LHS=lhsvarsnames()
  txt=lhsvars(LHS)+' = merror('+fid(1)+')'
  stk=list(list(' ','-2','1','1','10'),list(' ','-2','1','1','1'))
else
  stk=list('merror('+fid(1)+')','0','1','1','10')
end
if rhs==2 then
  txt=[txt;'mclearerr('+fid(1)+')']
end


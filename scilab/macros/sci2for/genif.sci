function [t]=genif(cnd,t1,t2)
//genif(cnd,txt) genere le if fortran relatif a la condition cnd et dont
//le  texte relatif  a la  partie  then est  donne  dans t1 et le  texte
//relatif a la partie else est eventuellement donne dans t2.
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==2&prod(size(t1))==1 then
  t=' if('+cnd+') '+t1
elseif rhs==3 then
  t=[' if('+cnd+') then';indentfor(t1);' else';indentfor(t2);' endif']
else
  t=[' if('+cnd+') then';indentfor(t1);' endif']
end
endfunction

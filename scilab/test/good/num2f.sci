function [stk,top]=num2f(val,stk)
// traduit la definition d'un nombre
//!
// Copyright INRIA
top=top+1
nn=length(val);typ='0'
for k=1:nn
  if part(val,k)=='.' then typ='1',break,end
end
stk(top)=list(val,'0',typ,'1','1')
endfunction

function [str]=pol2str(p)
//polynomial to character string
//
// p : polynomial (real)
// str : chracter string
//!
// Copyright INRIA
n=degree(p)
var=varn(p)
nv=length(var);while part(var,nv)==' ' then nv=nv-1,end;var=part(var,1:nv);
string(20)
p=coeff(p)
string(10)
if p(1)<>0 then
  if p(1)<0 then
    str=string(p(1))
  else
    str=' '+string(p(1))
  end
else
  str=' '
end
for k=1:n
  if p(k+1)<>0 then
    if p(k+1)<0 then
      str=str+'-'
    else
      str=str+'+'
    end
    if abs(p(k+1))<>1 then
      str=str+string(abs(p(k+1)))+'*'+var
    else
      str=str+var
    end
    if k>1 then str=str+'^'+string(k),end
  end
end
endfunction

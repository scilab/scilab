function [t]=gencall(arg)
//gencall(arg) genere une instruction call fortran  a partir du vecteur de
//chaine arg dont le premier element est le nom du sous programme et les
//suivants les arguments d'appels
//!
// Copyright INRIA
n=prod(size(arg))
t=' call '+arg(1)+'('
if n==0 then return,end
for k=2:n-1,
  t=t+arg(k)+','
end
t=t+arg(n)+')'
endfunction

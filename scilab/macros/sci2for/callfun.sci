function t=callfun(arg,typ)
//permet de generer le code fortran relatif a l'appel d'une fonction et
//gere la table des fonctions dont il faut specifier le type dans les 
//declarations
//!

// Copyright INRIA
[lhs,rhs]=argn(0)
n=prod(size(arg))
t=' '+arg(1)+'('
if n==0 then return,end
for k=2:n-1,
  t=t+arg(k)+','
end
t=t+arg(n)+')'
//
if rhs==3 then
  nwrk;
  select typ
  case '0' then
    if find(arg(1)==nwrk(14))==[] then  nwrk(14)=[nwrk(14);arg(1)],end
  case '1' then
    if find(arg(1)==nwrk(15))==[] then  nwrk(15)=[nwrk(15);arg(1)],end
   end
end
nwrk=resume(nwrk)
endfunction

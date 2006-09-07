function [h]=eval(z)
// Syntax :  H = eval(Z)
// returns the evaluation of the matrix of character strings Z.
// Example: a=1; b=2; Z=['a','b'] ; eval(Z) returns the matrix [1,2];
//!
// Copyright INRIA
[mz,nz]=size(z),
if mz*nz>1 then
   texte=[];
   for l=1:mz,for k=1:nz,
     texte=[texte;'%h('+string(l)+','+string(k)+')='+z(l,k)],
   end,end,
else
   texte='%h='+z
end
deff('[%h]=%eval()',texte),
h=%eval(),
endfunction

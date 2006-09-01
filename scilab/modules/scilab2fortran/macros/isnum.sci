function [x]=isnum(str)
// isnum(str) retourne la variable logique vrai si la chaine de caractere
// represente un nombre
//!
// Copyright INRIA
ok =[48:57,46,100,101,43,45] 
y=ascii(str);
x =%t 
for c=y; 
   x= (find(c==ok) <>[]) & x;
end 
endfunction

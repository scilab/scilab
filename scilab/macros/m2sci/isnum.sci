function [x]=isnum(str)
// isnum(str) retourne la variable logique vrai si la chaine de caractere
// represente un nombre
//!
// Copyright INRIA
 [H,ierr]=evstr(str)
 x=ierr==0
//x=find(part(str,1)==string(0:9))<>[];




function []=gaincom(pole,ue)
//[]=gaincom(pole,[ue])
// calcul de la matrice k de gain pour
// la commande du mod\`ele de competition
// de sorte que la matrice f -g*k ait pour p\^oles
// le vecteur colonne pole
//!
// Copyright INRIA
[lhs,rhs]=argn(0);
if rhs==1,ue=1,end
[f,g,h]=lincomp(ue);
k=ppol(f,g,pole)
[f,g,h,k]=resume(f,g,h,k)
endfunction

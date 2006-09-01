function []=gainobs(pole,ue)
//[]=gainobs(pole,[ue])
// calcul de la matrice l de gain pour
// l'observateur du modele de competition
// de sorte que la matrice f -l*h ait pour poles
// le vecteur colonne pole
//!
// Copyright INRIA
[lhs,rhs]=argn(0);
if rhs==1,ue=1,end
[f,g,h]=lincomp(ue);
l=ppol(f',h',pole)
l=l'
[f,g,h,l]=resume(f,g,h,l)
endfunction

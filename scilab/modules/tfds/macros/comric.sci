function []=comric(q1,q2,r)
//[]=comric(q1,q2,r)
// pi est la solution de l'equation de riccati stationnaire
// f'*pi+pi*f+ q*eye(2)-pi*g*(1/r)*g'*pi=0
// pour calculer le gain r^-1*g'*pi de la commande.
// ici, q et r sont des matrices de ponderation d'une fonction
// cout quadratique.
//!
// Copyright INRIA
[pi]=ricc(f,(1/r)*g*g',[q1,0;0,q2],'cont');
k= (1/r) *g'*pi;
[k]=resume(k);
endfunction

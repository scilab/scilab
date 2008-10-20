function []=obsric(q1,q2,r)
//[]=obsric(q1,q2,r)
// p est la solution de l'equation de riccati stationnaire
// f*p+p*f'+ q*eye(2)-(1/r)*p*h'*h*p=0
// pour calculer le gain r^-1*p*h' de l'observateur
// confondu avec le filtre de Kalman.
// ici, q est la variance du bruit d'etat,
//      r la variance du bruit de mesure.
//!
// Copyright INRIA
[p]=ricc(f',(1/r)*h'*h,[q1,0;0,q2],'cont');
l= (1/r) *p*h'
[l]=resume(l);
endfunction

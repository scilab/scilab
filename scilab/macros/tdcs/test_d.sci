function [f1,f2]=test_d(ue)
//[f1,f2]=test_d(ue)
// f1 est la dynamique linearisee
//autour du point d'equilibre donne par equilcom(ue)
//du systeme precedent observateur-controleur (voir simulcomp)
// f2 est la valeur theorique de f1 (voir cours)
//!
// Copyright INRIA
deff('[yy,zdot]=fff(z,vv)',...
    ['u=1-k*z(3:4)';
     'yy=0';
     'xdot(1,1) = ppr*z(1)*(1-z(1)/ppk) - u*ppa*z(1)*z(2)';
     'xdot(2,1) = pps*z(2)*(1-z(2)/ppl) - u*ppb*z(1)*z(2)';
     'xdot1 = (f-l*h-g*k)*z(3:4) + l*(z(1)-xe(1))';
     'zdot=[xdot;xdot1]']);
xe=equilcom(ue)
[f1,g1,h1,k1]=lin(fff,[xe;0;0],0),
f2=[ f, -g*k; l*h,f-l*h-g*k],
endfunction

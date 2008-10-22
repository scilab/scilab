function [cout,grad,ind]=iirlp(x,ind,p,flag,lambda,omega,ad,wa,td,wt)
//
//optimization of IIR filters IIR with  Lp criterium for magnitude
//                                      and/or group delay
//     -cf Rabiner & Gold pp270-273-
//
//auteur : G. Le Vey
//
//   p ===> critere Lp
//
//r=module des poles et zeros des filtres
//theta=argument des  "    "   "    "    "
//omega=frequences ou sont donnees les specifications des filtres
//wa,wt=fonctions de ponderation pour l'amplitude et le
//retard de groupe ad,td=amplitudes et retard de groupe desires
//!
// Copyright INRIA
r=x(:,1);theta=x(:,2);
[m,n]=size(ad);if m>n,ad=ad';end
[m,n]=size(td);if m>n,td=td';end
[m,n]=size(omega);if m>n,omega=omega';end;
[m,n]=size(r);if n>m,r=r';m=n;end;
[m,n]=size(theta);if n>m,theta=theta';m=n;end;
//
select flag
                case 'a'
//AMPLITUDE
[cout,grad]=iirmod(p,r,theta,omega,wa,ad);
//
                case 'gd'
//RETARD DE GROUPE
[cout,grad]=iirgroup(p,r,theta,omega,wt,td);
//
                 else
//AMPLITUDE ET RETARD DE GROUPE
[la,ga]=iirmod(p,r,theta,omega,wa,ad);
[lt,gt]=iirgroup(p,r,theta,omega,wt,td);
cout=lambda*la+(1-lambda)*lt;
grad=lambda*ga+(1-lambda)*gt;
end;
endfunction

function [co,u,g,itv,dtv]=icsegen(u,simu,nap,imp,ech,cof)
//Calcul du controle optimal avec mise a l'echelle du controle et 
//ponderation des observations
//Syntaxe
//[co,u,g,itv,dtv]=icsegen(u,simu,nap,imp,ech,cof)
//
//variables d'entree :
//u(nu)     : parametres initiaux
//simu      : chaine de caracteres donnant le nom du sous programme
//            decrivant le probleme (second menbre, critere et etat
//            initial)
//nap       : nombre maximum d' appels du simulateur
//imp       : valeur de debug pendant l'optimisation
//ech(1,nu) : coeff de mise a l'echelle du controle
//cof(1,ntob*nob) : coeff de ponderation des observations
//variables de sortie :
//co        : cout final
//u(nu)     : parametres finaux
//g(nu)     : gradient final
//itv(nitv) : tableau de travail (entiers fortran)
//dtv(ndtv) : tableau de travail (double precision fortran)
//Utiliser les macros icot et icob pour extraire l'etat
//!

// Copyright INRIA
if nu<large then alg='qn' ; else alg='gc' ; end;
itv=itu;
itv(nitv)=0;
dtv=[dtu,y0,tob,matrix(obs,1,ny*nob),don,ech,cof,b,fy1,fu1];
dtv(ndtv)=0;
[co,u,g,itv,dtv]=optim(simu,'b',binf,bsup,u, alg, df0, 'ar',nap,...
                       'ti',itv,'td',dtv,'si','sd')
endfunction




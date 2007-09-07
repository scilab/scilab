function [co,u,g,itv,dtv]=icse(u,simu,nap,imp)
//Calcul du controle optimal sans mise a l'echelle du
//controle et ponderation egale des observations
//variables d'entree :
//u(nu)     : parametres initiaux
//simu      : chaine de caracteres donnant le nom du sous programme
//            decrivant le probleme
//nap       : nombre maximum d' appels du simulateur
//imp       : valeur de debug pendant l'optimisation
//variables de sortie :
//co        : cout final
//u(nu)     : parametres finaux
//g(nu)     : gradient final
//itv(nitv) : tableau de travail (entiers fortran)
//dtv(ndtv) : tableau de travail (double precision fortran)
//Utiliser les macros icot et icob pour extraire l'etat
//!
// Copyright INRIA
df0=1;
nu=prod(size(u))
ech=ones(1,nu);
cof=ones(1,nob*ntob);
[co,u,g,itv,dtv]=icsegen(u,simu,nap,imp)
endfunction


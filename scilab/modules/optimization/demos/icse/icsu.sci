function [co,u,g,itv,dtv]=icsu(u,simu,nap,imp)
//Calcul du controle optimal avec ponderation du controle initial
//Une mise a l'echelle ramene toutes les composantes du controle
//initial a 1.Les composantes initialement nulles le resteront.
//Ne convient que si la borne inferieure est positive.
//%Syntaxe
//[co,u,g,itv,dtv]=icsu(u,simu,nap,imp)
//variables d'entree :
//u(nu)     : parametres initiaux
//simu      : chaine de caracteres donnant le nom du sous programme
//            decrivant le probleme (second menbre, critere et etat
//            initial)
//nap       : nombre maximum d' appels du simulateur
//imp       : valeur de debug pendant l'optimisation
//variables de sortie  :
//co        : cout final
//u(nu)     : parametres finaux
//g(nu)     : gradient final
//itv(nitv) : tableau de travail (entiers fortran)
//dtv(ndtv) : tableau de travail (double precision fortran)
//Utiliser les macros icot et icob pour extraire l'etat total
//ou l'etat aux instants de mesure de dtv.
//!
// Copyright INRIA
df0=1;
if mini(binf) <=0, ..
  error('appel de icsu avec binf non strictement positif'); end;
for i=1:nu,u(1,i)=maxi( [binf(1,i),mini([u(1,i),bsup(1,i)])] ), end;
ech=u;
binf=binf./u;
bsup=bsup./u;
u=ones(1,nu);
cof=ones(1,ntob*nob);
[co,u,g,itv,dtv]=icsegen(u,simu,nap,imp)
u=ech.*u;




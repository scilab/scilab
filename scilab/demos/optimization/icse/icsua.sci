function [co,u,g,itv,dtv,cof]=icsua(u,simu,nap,imp)
//Calcul du controle optimal avec ponderation du controle initial
//et ponderation arithmetique des poids des observations.
//Une mise a l'echelle ramene toutes les composantes du controle
//initial a 1.Les composantes initialement nulles le resteront.
//Ne convient que si la borne inferieure est positive.
//Formule de ponderation des poids :
//cof(i,j)=nex/(abs(ob(1,j,i)) + ... + abs(ob(nex,j,i)) )
//%Syntaxe
//[co,u,g,itv,dtv,cof]=icsua(u,simu,nap,imp)

// Copyright INRIA
//variables d'entree :
//u(nu) : parametres initiaux
//nap   : nombre maximum d' appels du simulateur
//imp       : valeur de debug pendant l'optimisation
//variables de sortie :
//co            : cout final
//u(nu)         : parametres finaux
//g(nu)         : gradient final
//itv(nitv)     : tableau de travail (entiers fortran)
//dtv(ndtv)     : tableau de travail (double precision fortran)
//cof(nob,ntob) : coefficients de ponderation du cout
//Utiliser les macros icot et icob pour extraire l'etat
//!
df0=1;
if mini(binf) <=0, ..
  error('appel de icsua avec binf non strictement positif'); end;
for i=1:nu,u(1,i)=maxi( [binf(1,i),mini([u(1,i),bsup(1,i)])] ), end;
ech=u;
binf=binf./u;
bsup=bsup./u;
u=ones(1,nu);
ico=1;
yob=0.d0*ones(nob,ntob);
ob=don;
[cof]=fort('icscof',ico,1,'i',ntob,2,'i',nex,3,'i',...
            nob,4,'i',yob,5,'d',ob,6,'d','sort',[1,nob*ntob],7,'d');
[co,u,g,itv,dtv]=icsegen(u,simu,nap,imp)
u=ech.*u




function [co,u,g,itv,dtv,cof]=icsuq(u,simu,nap,imp,obs,ytob)
//  Calcul du controle optimal avec ponderation du controle initial
//  et ponderation geometrique des poids des observations.
//  Une mise a l'echelle ramene toutes les composantes du controle
//  initial a 1.Les composantes initialement nulles le resteront.
//  Ne convient que si la borne inferieure est positive.
//  Formule de ponderation des poids : 
//  cof(i,j)=1/2*[(ytob(i,j)-ob(1,j,i))**2+..+(ytob(i,j)-ob(nex,j,i)**2]
//
//%Syntaxe
//[co,u,g,itv,dtv,cof]=icsuq(u,nap,imp,obs,ytob)
//variables d'entree :
//u(nu)         : parametres initiaux
//simu      : chaine de caracteres donnant le nom du sous programme
//            decrivant le probleme (second menbre, critere et etat
//            initial)
//nap           : nombre maximum d' appels du simulateur
//imp       : valeur de debug pendant l'optimisation
//obs(nob,ny)   : matrice d'observation
//ytob(ny,ntob) : valeurs initiales de l'etat aux instants de
//mesure obtenues par icob apres la mise en
//oeuvre de icse,icsu,icsua ou icsuq
//variables de sortie :
//co            : cout final
//u(nu)         : parametres finaux
//g(nu)         : gradient final
//itv(nitv)     : tableau de travail (entiers fortran)
//dtv(ndtv)     : tableau de travail (double precision fortran)
//cof(nob,ntob) : coefficients de ponderation du cout
//Utiliser les macros icot et icob pour extraire l'etat
//!
// Copyright INRIA
df0=1;
if mini(binf) <=0, ..
  error('appel de icsuq avec binf non strictement positif'); end;
for i=1 : nu, u(1,i)=maxi([ binf(1,i),mini([u(1,i),bsup(1,i)])] ), end;
ech=u;
binf=binf./u;
bsup=bsup./u;
u=ones(1,nu);
ico=2;
yob=obs*ytob;
ob=don;

[cof]=fort('icscof',ico,1,'i',ntob,2,'i',nex,3,'i',...
            nob,4,'i',yob,5,'d',ob,6,'d','sort',[1,nob*ntob],7,'d');
[co,u,g,itv,dtv]=icsegen(u,simu,nap,imp)
u=ech.*u



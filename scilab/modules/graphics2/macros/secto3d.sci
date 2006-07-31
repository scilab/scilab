function [m,x]=secto3d(seclist,npas)
//Etant donnee une surface 3D decrite par une liste (seclist) de sections
// dans le plan  (x,z),  [m [,x]]=secto3d(seclist [,npas]) retourne une
// matrice m  contenant une discretisation reguliere de la surface:
//   la i-ieme ligne de la matrice m correspond a la i-ieme section,
//   la j-ieme colonne de m  correspondant a l'abscisse x(j).
//Chaque section seclist(i) est decrite par une matrice a 2 lignes,
// correspondants respectivement aux coordonnees selon l'axe des x et des z.
//%methode
// Il est tout d'abord calcule les bornes extremales xmin,xmax sur
// l'axe des x pour l'ensemble des sections, l'intervalle [xmin,xmax]
// est ensuite decoupe en npas egaux, chaque section est ensuite interpolee
// lineairement en chaque point de cette discretisation.
//
// si npas est omis il est par defaut pris egal au nombre de sections.
//%autre syntaxe
// [m]=secto3d(seclist ,x) dans ce cas le vecteur x sera pris comme decoupage
// de l'axe des x pour l'ensemble des sections
//%voir aussi
// la macro plot3d
//!
//origine S Steer INRIA 1991
// Copyright INRIA

[lhs,rhs]=argn(0)
n=size(seclist)
//
if rhs<2 then npas=n,end
if prod(size(npas))==1 then
  sec=seclist(1);sec=sec(1,:)
  xmax=maxi(sec);xmin=mini(sec)
  for k=2:n
    sec=seclist(k);sec=sec(1,:)
    xmax=maxi(xmax,maxi(sec))
    xmin=mini(xmin,mini(sec))
  end
  pas=(xmax-xmin)/npas
  x=[xmin-pas xmin:pas:xmax xmax+pas]
else
  x=npas
end
nx=prod(size(x))
m(n,nx)=0;
for k=1:n
  sec=seclist(k)
  [ms,ns]=size(sec)
  sec=[[x(1);sec(2,1)],sec,[x(nx);sec(2,ns)]]
  m(k,:)=interpln(sec,x);
end
endfunction

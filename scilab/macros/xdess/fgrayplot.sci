function []=fgrayplot(x,y,f,strf,rect,nax,void)
//[]=fgrayplot(x,y,f)
// Trace en niveau de gris une surface
// z=f(x,y) d\'efinie par un external f ( ex macro [y]=f(x)) 
// on calcule d'abord f sur la grille definie par x.y
// x et y sont des vecteurs implicites donnant les
// abscisses et les ordonn\'ees des points de la grille
// le niveau de gris choisi sur chaque maillage depend de la valeur
//    moyenne de z dans le maillage.
// Exemple : taper fgrayplot() pour voir un exemple .
//    deff('[z]=surf(x,y)','z=x**2+y**2');
//    fgrayplot(-1:0.1:1,-1:0.1:1,surf);
//!
// Copyright INRIA
[lhs,rhs]=argn(0);
if rhs<=0,
  s_mat=['deff(''[z]=surf(x,y)'',''z=x**2+y**2'');';
      'fgrayplot(-1:0.1:1,-1:0.1:1,surf);'];
  write(%io(2),s_mat);execstr(s_mat);
  return;
end;
opts=[]
if exists('style','local')==1 then opts=[opts,'style=style'],end
if exists('strf','local')==1 then opts=[opts,'strf=strf'],end
if exists('rect','local')==1 then opts=[opts,'rect=rect'],end
if exists('nax','local')==1 then opts=[opts,'nax=nax'],end
if exists('frameflag','local')==1 then opts=[opts,'frameflag=frameflag'],end
if exists('axesflag','local')==1 then opts=[opts,'axesflag=axesflag'],end
if size(opts,2)<rhs-3 then  error('invalid named arguments'),end

if type(f)==11 then comp(f),end;
execstr('grayplot(x,y,feval(x,y,f),'+strcat(opts,',')+')')
endfunction

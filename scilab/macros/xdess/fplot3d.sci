function fplot3d(xr,yr,f,theta,alpha,leg,flag,ebox)
// fplot3d(xr,yr,f,teta,alpha,leg,[flag,ebox])
// Trace la surface d\'efinie par un external f ( ex macro [z]=f(x,y))
// on calcule d'abord f sur la grille definie par xr.yr
// xr et yr sont des vecteurs implicites donnant les
// abscisses et les ordonn\'ees des points de la grille
// -teta, alpha : sont les angles en coordonn\'ees spheriques du
//      point d'observation
// -flag et ebox (voir plot3d)
// Exemple : taper fplot3d() pour voir un exemple
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<=0 then
  s_mat=['deff(''[z]=surf(x,y)'',''z=sin(x)*cos(y)'');';
      't=-%pi:0.3:%pi;';
      'fplot3d(t,t,surf,35,45,'"X@Y@Z'");'];
  write(%io(2),s_mat);execstr(s_mat);
  return;
end;
if rhs<3, error(' I need at least 3 arguments'),end;

opts=[]
if exists('theta','local')==1 then opts=[opts,'theta=theta'],end
if exists('alpha','local')==1 then opts=[opts,'alpha=alpha'],end
if exists('leg'  ,'local')==1 then opts=[opts,'leg=leg']    ,end
if exists('flag' ,'local')==1 then opts=[opts,'flag=flag']  ,end
if exists('ebox' ,'local')==1 then opts=[opts,'ebox=ebox']  ,end
if size(opts,2)<rhs-3 then  error('invalid named arguments'),end
if type(f)==11 then comp(f),end;

execstr('plot3d(xr,yr,feval(xr,yr,f),'+strcat(opts,',')+')')
endfunction

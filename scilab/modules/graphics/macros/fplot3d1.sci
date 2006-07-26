function []=fplot3d1(xr,yr,f,theta,alpha,leg,flag,ebox)
// Trace la surface d\'efinie par un external f ( ex macro [z]=f(x,y))
// on calcule d'abord f sur la grille definie par xr.yr
// xr et yr sont des vecteurs implicites donnant les
// abscisses et les ordonn\'ees des points de la grille
// -theta, alpha : sont les angles en coordonn\'ees spheriques du
//      point d'observation
// -flag et ebox (voir plot3d1)
// Exemple : taper fplot3d1() pour voir un exemple
// deff('<z>=Surf(x,y)','z=x**2+y**2');
// res=fplot3d1(Surf,-1:0.1:1,-1:0.1:1,35,45,"X@Y@Z");
//!
// Copyright INRIA
	
	[lhs,rhs]=argn(0)
	if rhs <= 0 then   // demo
		
		title_demo = [
			'';
			'Demo of fplot3d1()';
			'========================================';
			''];
		
		s_mat=[
			'deff(''[z]=Surf(x,y)'',''z=sin(x)*cos(y)'');';
			't=-%pi:0.3:%pi;';
			'fplot3d1(t,t,Surf,35,45,'"X@Y@Z'");'];
		
		write(%io(2),title_demo);
		write(%io(2),s_mat);
		write(%io(2),' ');
		execstr(s_mat);
		return
	end

if rhs<3, error(' I need at least 3 arguments'),end;

opts=[]
if exists('theta','local')==1 then opts=[opts,'theta=theta'],end
if exists('alpha','local')==1 then opts=[opts,'alpha=alpha'],end
if exists('leg'  ,'local')==1 then opts=[opts,'leg=leg']    ,end
if exists('flag' ,'local')==1 then opts=[opts,'flag=flag']  ,end
if exists('ebox' ,'local')==1 then opts=[opts,'ebox=ebox']  ,end
if size(opts,2)<rhs-3 then  error('invalid named arguments'),end
if type(f)==11 then comp(f),end;

execstr('plot3d1(xr,yr,feval(xr,yr,f),'+strcat(opts,',')+')')
endfunction

// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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

	
	[lhs,rhs]=argn(0)
	if rhs <= 0 then   // demo
		deff("[z]=Surf(x,y)","z=sin(x)*cos(y)");
		t=-%pi:0.3:%pi;
		fplot3d1(t,t,Surf,35,45,"X@Y@Z");
		return
	end

if rhs<3 then
  error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "fplot3d1", 3));
end;

opts=[]
if exists('theta','local')==1 then opts=[opts,'theta=theta'],end
if exists('alpha','local')==1 then opts=[opts,'alpha=alpha'],end
if exists('leg'  ,'local')==1 then opts=[opts,'leg=leg']    ,end
if exists('flag' ,'local')==1 then opts=[opts,'flag=flag']  ,end
if exists('ebox' ,'local')==1 then opts=[opts,'ebox=ebox']  ,end
if type(f)==11 then comp(f),end;

execstr('plot3d1(xr,yr,feval(xr,yr,f),'+strcat(opts,',')+')')
endfunction

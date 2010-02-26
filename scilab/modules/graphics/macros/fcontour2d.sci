// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function fcontour2d(xr,yr,f,nz,style,strf,leg,rect,nax,void)
	
	// deff('[z]=Surf(x,y)','z=x**2+y**2');
	// fcontour(Surf,-1:0.1:1,-1:0.1:1,10);
	
	
	[lhs,rhs]=argn(0);
	
	if rhs == 0 then   // demo
		deff("[z]=Surf(x,y)","z=x**3+y");
		fcontour2d(-1:0.1:1,-1:0.1:1,Surf,10,style=1:10,rect=[-1,-1,1,1]*1.5,strf="011");
		return
	end

if rhs<3,
    error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "fcontour2d", 3));
end

opts=[]
if exists('nz','local')==0 then nz=10,rhs=rhs+1,end
if exists('style','local')==1 then opts=[opts,'style=style'],end
if exists('strf','local')==1 then opts=[opts,'strf=strf'],end
if exists('leg','local')==1 then opts=[opts,'leg=leg'],end
if exists('rect','local')==1 then opts=[opts,'rect=rect'],end
if exists('nax','local')==1 then opts=[opts,'nax=nax'],end
if exists('frameflag','local')==1 then opts=[opts,'frameflag=frameflag'],end
if exists('axesflag','local')==1 then opts=[opts,'axesflag=axesflag'],end

if type(f)==11 then comp(f),end;
execstr('contour2d(xr,yr,feval(xr,yr,f),nz,'+strcat(opts,',')+')')
endfunction

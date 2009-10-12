// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function []=fplot2d(xr,f,style,strf,leg,rect,nax,void)
	// 2D plot of function f : a Scilab function or the name (as a string)
	//  of a dynamically linked function.
	// Enter fplot2d() for an example.
	// deff('<y>=f(x)','y=sin(x)+cos(x)');
	// fplot2d(f,0:0.1:%pi);
	//!
	
	[lhs,rhs]=argn(0)
	if rhs <= 0 then   // demo
		deff("[y]=f(x)","y=sin(x)+cos(x)");
		fplot2d(0:0.1:%pi,f);
		return
	end
	
	if type(xr)=='10' then // logflag passed first
    error(msprintf(gettext("%s: Wrong size for input argument #%d: A vector expected.\n"), "fplot2d", 1));
	elseif rhs < 2 then 
    error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "fplot2d", 2));
	end
	//handling optionnal arguments
	
	opts=[]
	
	if exists('style','local')==1 then opts=[opts,'style=style'],end
	if exists('strf','local')==1 then opts=[opts,'strf=strf'],end
	if exists('leg','local')==1 then opts=[opts,'leg=leg'],end
	if exists('rect','local')==1 then opts=[opts,'rect=rect'],end
	if exists('nax','local')==1 then opts=[opts,'nax=nax'],end
	if exists('logflag','local')==1 then opts=[opts,'logflag=logflag'],end
	if exists('frameflag','local')==1 then opts=[opts,'frameflag=frameflag'],end
	if exists('axesflag','local')==1 then opts=[opts,'axesflag=axesflag'],end
	if size(opts,2)<rhs-2 then  error('invalid named arguments'),end
	
	execstr('plot2d(xr,feval(xr,f),'+strcat(opts,',')+')')
	
endfunction

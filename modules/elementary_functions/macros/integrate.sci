// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x=integrate(expr,var,x0,x1,ea,er)
// x=integrate(expr,v,x0,x1 [,ea [,er]])  computes
//                      /x1
//                     [
//                 x = I  f(v)dv
//                     ]
//                    /x0
//
//
//examples:
//integrate('sin(x)','x',0,%pi)
//integrate(['if x==0 then 1,';
//           'else sin(x)/x,end'],'x',0,%pi)

	[lhs,rhs]=argn(0)
	if size(x0,'*')<>1 then
		error(msprintf(gettext("%s: Wrong size for third input argument: Real scalar expected.\n"),"integrate"));
	end
	if imag(x0)<>0 then
		error(msprintf(gettext("%s: Wrong value for third input argument: Real scalar expected.\n"),"integrate"));
	end
	[m,n]=size(x1),x1=matrix(x1,1,m*n)
	if norm(imag(x1),1)<>0 then
		error(msprintf(gettext("%s: Wrong value for fourth input argument: Real scalar expected.\n"),"integrate"));
	end
	//
	deff('[ans]=func('+var+')',expr,'n')
	x=[]
	select rhs
	case 4 then for xk=x1,x=[x,intg(x0,xk,func)],end,
	case 5 then for xk=x1,x=[x,intg(x0,xk,func,ea)],end,
	case 6 then for xk=x1,x=[x,intg(x0,xk,func,ea,er)],end,
	else error(39), end,
	x=matrix(x,m,n)
endfunction

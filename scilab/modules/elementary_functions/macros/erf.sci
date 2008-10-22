// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function y=erf(x)
	
	//erf  -  The error function.
	//%Syntax
	// y = erf(x).
	//%Parameters
	// x : real vector
	// y : real vector (of same size)
	//%Description
	// erf computes the error function:
	//
	//                      /x
	//      y = 2/sqrt(pi) *|  exp(-t^2) dt
	//                      /0
	//%See also
	// erfc erfcx calerf
	//!
	
	rhs = argn(2);
	
	// check the number of input arguments
	if rhs<>1 then
		error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"erf",1));
	end
	
	y=calerf(x,0);
	
endfunction

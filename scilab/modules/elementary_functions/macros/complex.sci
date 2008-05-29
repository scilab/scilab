// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function  y=complex(varargin)

// COMPLEX function
// Given the real part and the imaginary part this function constructs the complex form : y=complex(a,b) returns y=a+b*i
// Inputs :
//  a, b : real scalars/vectors/matrices
// output :
// y : complex form

	rhs = argn(2);
	
	if rhs == 1 then
		a = varargin(1);
		if isreal(a) then
			y = a+imult(zeros(a));
		else
			error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"complex"));
		end
	elseif rhs == 2 then
		a = varargin(1);
		b = varargin(2);
		if isreal(a) & isreal(b) then
			y = a+imult(b);
		elseif ~ isreal(a) then
			error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"complex",1));
		else
			error(msprintf(gettext("%s: Wrong type for input argument #%d: Real matrix expected.\n"),"complex",2));
		end
	else
		error(msprintf(gettext("%s: Wrong number of input argument(s): %d or %d expected.\n"),"complex",1,2));
	end
	
endfunction

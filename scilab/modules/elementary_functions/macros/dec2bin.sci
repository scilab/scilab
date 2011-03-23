// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// =============================================================================
//
// Author : F.Belahcene
// DEC2BIN function
//
// Given x, a positive scalar/vector/matix of reals, this function returns
// a column vector of strings. Each string is the binary representation
// of the input argument components (i.e y(i) is the binary representation
// of x(i))
//
// -Inputs :
//    x : a  scalar/vector/matix of positives reals
//    n : an integer
// -Output :
//    y : a vector of strings (positives)
//
// =============================================================================

function y=dec2bin(x,n)
	
	rhs = argn(2);
	
	// check the number of input arguments
	if (rhs<1 | rhs>2) then
		error(msprintf(gettext("%s: Wrong number of input argument(s): %d or %d expected.\n"),"dec2bin",1,2));
	end
	
	// check type and size of the input arguments
	if or(type(x)<>8) & (or(type(x)<>1) | or(x<0)) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: Scalar/vector/matrix of positive integers expected.\n"),"dec2bin",1));
	end
	
	if rhs==2 & ((type(n)<>8 & (type(n)<>1 | n<0)) | prod(size(n))<>1) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: A positive integer expected"),"dec2bin",2));
	end
	
	// empty matrix
	if x==[]
		y=string([]);
		return;
	end
	
	[nr,nc] = size(x);
	x=x(:);
	
	// input argument is a scalar/vector/matrix of zeros
	
	if and(x==0)
		if rhs==2
			y = strcat(string(zeros(1:n))) + emptystr(nr,nc);
		else
			y = "0" + emptystr(nr,nc);
		end
		return
	end
	
	// for x=25, pow=[4 3 0], because x=2^4+2^3+2^0
	// for x=23, pow=[4 2 1 0] because x=2^4+2^2+2^1+2^0
	// for x=[25 23]
	// pow=[4 3 0 -1
	//      4 2 1 0];
	
	while find(x>0)<>[]
		pow(x>0,$+1) = floor(log2(double(x(x>0))));
		pow(x<=0,$)  = -1;
		x(x>0)       = floor(x(x>0)-2^pow(x>0,$));
	end
	
	pow   = pow+1;
	ytemp = zeros(size(pow,1),size(pow,2));
	
	for i=1:size(ytemp,1)
		ind          = pow(i,pow(i,:)>=1);
		ytemp(i,ind) = 1;
	end
	
	if rhs==2
		for i=1:size(ytemp,1)
			y(i)=strcat(string([zeros(1,round(n-size(ytemp,2))) ytemp(i,size(ytemp,2):-1:1)]));
		end
	else
		for i=1:size(ytemp,1)
			y(i)=strcat(string(ytemp(i,size(ytemp,2):-1:1)));
		end
	end
	
	y = matrix(y,nr,nc);
	
endfunction

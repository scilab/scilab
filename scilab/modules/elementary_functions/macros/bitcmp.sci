function y = bitcmp(x,n)
	
	// Copyright INRIA
	// BITCMP function
	//
	// Given an unsigned integer x, this function returns the unsigned integer
	// which is the integer corresponding to the complementary of the binary
	// form of x
	//
	// If the bits number of the x binary representation is less than the
	// bitmax number (8,16 or 32) then the bits '1' are added to the
	// complementary in order to have bitmax number (8, 16 or 32) for the
	// complementary
	//
	// for example for the type uint8 (bitmax=8), the complementary of '1101' is not '0010' but '11110010'
	// The integer n sets the bits max number
	// -Inputs :
	//  x : an unsigned integer
	//  n : a positive integer between 1 and the bitmax of the x type
	//
	// -Output :
	//  y : an unsigned integer
	//
	// F.Belahcene
	//
	// P. Marechal, 5 Feb 2008
	//   - Add argument check
	
	// Check input arguments
	// =========================================================================
	
	rhs = argn(2);
	
	// check number input argument
	
	rhs = argn(2);
	if (type(x) == 1) & (rhs == 1) then
		error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"bitcmp",2));
	elseif rhs == 0 then
		error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"),"bitcmp",1));
	end
	
	// check type
	
	if    (type(x)==1  & (x-floor(x)<>0 | x<0)) ..
		| (type(x)==8  & (inttype(x)<10)) ..
		| (type(x)<>1  & type(x)<>8) then
		
		error(msprintf(gettext("%s: Wrong first input argument: Scalar/matrix/hypermatrix of unsigned integers expected.\n"),"bitcmp"));
	end
	
	
	
	if  (rhs == 2) & ( ..
			(type(n)==1  & (n-floor(n)<>0 | x<0)) ..
			| (type(n)==8  & (inttype(n)<10)) ..
			| (type(n)<>1  & type(n)<>8) ..
			| (size(n,"*")<>1) ) then
		
		error(msprintf(gettext("%s: Wrong second input argument: unsigned integer expected.\n"),"bitcmp"));
	end
	
	// check n value
	
	if type(x)==8 then
		select inttype(x)
			case 11 then nmax = 8;
			case 12 then nmax = 16;
			case 14 then nmax = 32;
		end
	else
		nmax = 53;
	end
	
	if rhs == 2 then
		if n > nmax then
			error(msprintf(gettext("%s: Wrong value for second input argument: Must be between %d and %d.\n"),"bitcmp",0,nmax));
		end
		
	elseif rhs == 1 then
		n = nmax;
	end
	
	// case empty matrix
	
	if isempty(x) then
		y=[];
		return;
	end
	
	
	// Algorithm
	// =========================================================================
	
	xbin = dec2bin(x,nmax)
	xbin = part(xbin,nmax-n+1:length(xbin(1)))
	xbin = strsubst(xbin,"1","2");
	xbin = strsubst(xbin,"0","1");
	xbin = strsubst(xbin,"2","0");
	y    = bin2dec(xbin)
	
	
	// Result
	// =========================================================================
	
	if type(x)==8 then
		select inttype(x)
			case 11 then y = uint8(y);
			case 12 then y = uint16(y);
			case 14 then y = uint32(y);
		end
	end
	
	y = matrix(y,size(y));
	
endfunction

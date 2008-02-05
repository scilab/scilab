function y=bitget(x,pos)
	
	// Copyright INRIA
	// BITGET function
	// Given an unsigned integer x, this function returns an unsigned integer
	// (0 or 1) which is the bit number 'pos' from the representation binary of x
	// if x=uint8(19) and pos=2 then bitget returns the 2th bit of the binary form of 19 ('10011') which is 1
	// -Inputs :
	//  x : an unsigned integer
	// pos : a positive integer between 1 and the bitmax of the x type
	//
	// -Output :
	//  y : an unsigned integer 
	//
	// F.Belahcene
	
	// P. Marechal, 5 Feb 2008
	//   - Add argument check
	
	// Check input arguments
	// =========================================================================
	
	// check number input argument
	
	rhs = argn(2);
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"bitget",2));
	end
	
	// case empty matrix
	
	if isempty(x)
		if ~isempty(pos) & prod(size(pos))<>1
			error(msprintf(gettext("%s: Wrong size for input arguments: Same sizes expected.\n"),"bitget"));
		else
			y=[]
			return
		end
	end
	
	// check size
	
	if (size(x,"*")>1) & (size(pos,"*")>1) & (or(size(x)<>size(pos))) then
		error(msprintf(gettext("%s: Wrong size for input arguments: Same sizes expected.\n"),"bitget"));
	end
	
	if    (type(x)==1  & (x-floor(x)<>0 | x<0)) ..
		| (type(x)==8  & x<0) ..
		| (type(x)<>1  & type(x)<>8) then
		
		error(msprintf(gettext("%s: Wrong first input argument: Scalar/matrix/hypermatrix of positive integers expected.\n"),"bitget"));
	end
	
	// check type
	
	if    (type(pos)==1  & (pos-floor(pos)<>0 | pos<0)) ..
		| (type(pos)==8  & pos<0) ..
		| (type(pos)<>1  & type(pos)<>8) then
		
		error(msprintf(gettext("%s: Wrong second input argument: Scalar/matrix/hypermatrix of positive integers expected.\n"),"bitget"));
	end
	
	// Algorithm
	// =========================================================================
	
	if size(pos,"*") == 1;
		pos  = ones(x)*pos;
	end
	
	if size(x,"*") == 1;
		x    = ones(pos)*x;
	end
	
	xbin = strrev(dec2bin(x,max(pos)));
	
	for i=1:size(xbin,"*")
		ytemp = asciimat(xbin(i))-48;
		y(i)  = ytemp(pos(i));
	end
	
	// Result
	// =========================================================================
	
	if type(x)==8 then
		select inttype(x)
			case 1  then y = int8(y),
			case 2  then y = int16(y),
			case 4  then y = int32(y),
			case 11 then y = uint8(y),
			case 12 then y = uint16(y),
			case 14 then y = uint32(y),
		end
	end
	
	y = matrix(y,siye(x));
	
endfunction

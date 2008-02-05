function z=bitxor(x,y)
	
	// Copyright INRIA
	// BITXOR function
	// Given x,y two positives integers this function returns the decimal number
	// whose the binary form is the XOR of the binary representations of x and y
	
	// -Inputs :
	//  x, y :  scalars/vectors/matices/hypermatices of positives integers, 
	//          x and y must have the same size
	// -Output :
	//  z : scalar/vector/matrix/hypermatice
	//
	// P. Marechal, 5 Feb 2008
	//   - fix bug 2691 and 2692
	//   - Add argument check
	
	// Check input arguments
	// =========================================================================
	
	rhs = argn(2);
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"bitxor",2));
	end
	
	if typeof(x)<>typeof(y)
		error(msprintf(gettext("%s: Wrong type for inputs arguments: Same type expected.\n"),"bitxor"));
	end
	
	if or(size(x)<>size(y)) then
		error(msprintf(gettext("%s: Wrong size for input arguments: Same sizes expected.\n"),"bitxor"));
	end
	
	if isempty(x) & isempty(x)
		z=[];
		return
	end
	
	if    (type(x)==1  & (x-floor(x)<>0 | x<0)) ..
		| (type(x)==8  & x<0) ..
		| (type(x)==17 & (type(x.entries<>1) | type(x.entries<>8)) & find(x.entries>0)<>[]) ..
		| (type(x)<>1  & type(x)<>8 & type(x)<>17) then
		
		error(msprintf(gettext("%s: Wrong first input argument: Scalar/matrix/hypermatrix of positive integers expected.\n"),"bitxor"));
	end
	
	if    (type(y)==1  & (y-floor(y)<>0 | y<0)) ..
		| (type(y)==8  & y<0) ..
		| (type(y)==17 & (type(y.entries<>1) | type(y.entries<>8)) & find(y.entries>0)<>[]) ..
		| (type(y)<>1  & type(y)<>8 & type(y)<>17) then
		
		error(msprintf(gettext("%s: Wrong second input argument: Scalar/matrix/hypermatrix of positive integers expected.\n"),"bitxor"));
	end
	
	// Algorithm
	// =========================================================================
	
	for i=1:prod(size(x))
		zbin=dec2bin([x(i);y(i)])
		zand=strcat((string(sum(asciimat(zbin)-48,1))))
		zand=strsubst(zand,'2','0')
		z(i)=bin2dec(zand)
	end
	
	// Result
	// =========================================================================
	
	if type(x)==8 then
		select inttype(x)
			case 1  then z = int8(z),
			case 2  then z = int16(z),
			case 4  then z = int32(z),
			case 11 then z = uint8(z),
			case 12 then z = uint16(z),
			case 14 then z = uint32(z),
		end
	end
	
	z = matrix(z,size(x));
	
endfunction

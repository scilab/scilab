// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function y = bitset(x,pos,v)
	
	// INRIA 2008 - Pierre MARECHAL
	//
	// BITSET function
	// Set bit at specified position
	
	// Check input arguments
	// =========================================================================
	
	// check number input argument
	
	rhs = argn(2);
	
	if rhs < 2 then
		error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"),"bitset",2));
	end
	
	// case empty matrix
	
	if isempty(x)
		if ~isempty(pos) & prod(size(pos))<>1
			error(msprintf(gettext("%s: Wrong size for input arguments: Same sizes expected.\n"),"bitset"));
		else
			y=[]
			return
		end
	end
	
	// check size
	
	if (size(x,"*")>1) & (size(pos,"*")>1) & (or(size(x)<>size(pos))) then
		error(msprintf(gettext("%s: Wrong size for input arguments: Same sizes expected.\n"),"bitset"));
	end
	
	// check type
	
	if    (type(x)==1  & (x-floor(x)<>0 | x<0)) ..
		| (type(x)==8  & (inttype(x)<10)) ..
		| (type(x)<>1  & type(x)<>8) then
		
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Scalar/matrix of unsigned integers expected.\n"),"bitset",1));
	end
	
	if    (type(pos)==1  & (pos-floor(pos)<>0 | pos<0)) ..
		| (type(pos)==8  & (inttype(pos)<10)) ..
		| (type(pos)<>1  & type(pos)<>8) then
		
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Scalar/matrix of unsigned integers expected.\n"),"bitset",2));
	end
	
	// check pos value
	
	select inttype(x)
		case 0  then posmax = 52;
		case 11 then posmax = 8;
		case 12 then posmax = 16;
		case 14 then posmax = 32;
	end
	
	if (pos>posmax) | (pos<1) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be between %d and %d.\n"),"bitset",2,1,posmax));
	end
	
	// check v value
	
	if rhs == 3 & ..
		( ((type(v)<>1) & (type(v)<>8)) ..
		| ((type(x)==8) & (inttype(x)<10)) ..
		| ((v<>0) & (v<>1)) ) then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: 0 or 1 expected\n"),"bitset",3));
	end
	
	// Algorithm
	// =========================================================================
	
	if size(pos,"*") == 1;
		pos  = ones(x)*pos;
	end
	
	if size(x,"*") == 1;
		x    = ones(pos)*x;
	end
	
	if rhs<3 then
		v = 1;
	end
	
	if type(x)==8 then
		
		select inttype(x)
			case 11 then mask = uint8(2^(pos-1));
			case 12 then mask = uint16(2^(pos-1));
			case 14 then mask = uint32(2^(pos-1));
		end
		
		if v==0 then
			y = x & (~mask);
		else
			y = x | mask;
		end
		
		return;
		
	else
		
		// type == 1
		
		a     = 2^32;
		mask  = uint32(zeros(pos));
		
		y_MSB  = uint32(zeros(pos));
		y_LSB  = uint32(zeros(pos));
		
		y_LSB = uint32( x - double(uint32(x/a)) * a ); // LSB Less Significant Bits
		y_MSB = uint32( x/a );                         // MSB Most Significant Bits
		
		if or(pos<=32) then
			mask(  pos<=32 ) = uint32( 2^(pos(pos<=32) -1 ));
			if v==0 then
				y_LSB( pos<=32 ) = y_LSB(pos<=32) & (~mask(pos<=32));
			else
				y_LSB( pos<=32 ) = y_LSB(pos<=32) | mask(pos<=32);
			end
		end
		
		if or(pos>32) then
			mask(  pos>32 ) = uint32( 2^(pos(pos>32) -32 -1 ));
			if v==0 then
				y_MSB( pos>32 ) = y_MSB(pos>32) & (~ mask(pos>32));
			else
				y_MSB( pos>32 ) = y_MSB(pos>32) | mask(pos>32);
			end
		end
		
		y = double( a * y_MSB + y_LSB );
		
	end
	
endfunction

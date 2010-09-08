// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Farid BELAHCENE
// Copyright (C) 2006 - INRIA - Pierre MARECHAL
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// =============================================================================
//
// BIN2dec function
// Given str a binary string, this function returns the decimal number whose the
// binary representation is given by str
//
// -Input :
//    str : a string (or a vector/matrix of strings), containing only characters
//         '1' and '0'
// -Output :
//    y : a scalar/vector/matrix
//
// F.Belahcene

// check the type of input argument

// 2006-06-26 : Modified by Pierre MARECHAL
// Check length of given string ( must be 47 bits or less )
// =============================================================================

function y=bin2dec(str)

	if type(str)<>10
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"),"bin2dec",1));
	end
	
	// delete all spaces included in the str
	str = strsubst(str,' ','');
	
	// check the str characters are only '0' or '1', and convert the binary str to corresponing decimal number
	for i=1:prod(size(str))
		
		ind1=strindex(str(i),'1')
		ind0=strindex(str(i),'0')
		
		if length(str(i)) <> sum([prod(size(ind0)) prod(size(ind1))]) then
			error(msprintf(gettext("%s: Wrong value for input argument #%d: Matrix of strings made of zeros and ones expected.\n"),"bin2dec",1));
		end
		
		if length(str(i)) > 54 then
			error(msprintf(gettext("%s: Wrong size for input argument #%d: Must be less than %d characters.\n"),"bin2dec",1,54));
		end
		
		if ~isempty(ind1)
			ind1   = length(str(i))-ind1($:-1:1);
			y($+1) = sum(2^ind1);
		elseif ~isempty(ind0)
			y($+1) = 0;
		else
			y($+1) = [];
		end
	end
	
	y=matrix(y,size(str));
	
endfunction

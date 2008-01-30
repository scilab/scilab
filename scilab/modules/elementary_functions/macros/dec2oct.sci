// =============================================================================
// Copyright INRIA
// Author : Pierre MARECHAL
// dec2oct function
// =============================================================================

function y = dec2oct(x)
	
	if or(type(x)<>8) & (or(type(x)<>1) | or(x<0) ) then
		error(msprintf(gettext("%s: Wrong value for first input argument: Scalar/vector/matrix of positive integers expected.\n"),"dec2oct"));
	end
	
	[nr,nc] = size(x);
	y       = emptystr(nr,nc);
	
	for i=1:nr
		for j=1:nc
			
			x_bin          = dec2bin(x(i,j));
			x_bin_length   = length(x_bin);
			
			// Add 0 to the right of the string
			while modulo(length(x_bin),3) != 0 ,
				x_bin = '0' + x_bin;
			end
			
			x_bin_splitted = strsplit(x_bin,3*[1:(length(x_bin)/3-1)]);
			y(i,j)         = code2str(bin2dec(x_bin_splitted));
			
		end
	end
	
endfunction

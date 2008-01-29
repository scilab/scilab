// =============================================================================
// Copyright INRIA
// oct2dec function
//
// oct2dec(o) returns in vector d the numbers corresponding to the
// octal representation of o.
//
// -Input :
//    str : a string (or a vector/matrix of strings)
// -Output :
//    y : a scalar/vector/matrix
//
// =============================================================================

function d=oct2dec(o)
	
	if type(o)<> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument: Matrix of strings expected.\n"),"oct2dec"));
	end
	
	[nr,nc] = size(o)
	n       = length(o)
	p       = cumprod([1,8*ones(1,max(n)-1)]);
	d       = zeros(o);
	
	for i=1:nr
		for j=1:nc
			s = abs(str2code(o(i,j)));
			if max(s)>7 then
				error(msprintf(gettext("%s: Wrong value for input argument(s): Valid octal representations expected.\n"),"oct2dec"));
			end
			d(i,j) = p(n(i,j):-1:1)*s;
		end
	end
	
endfunction

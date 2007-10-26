function y=primes(x)
// Copyright INRIA
// PRIMES function
// Given a real x, this function returns a vector whose the components are all the primes numbers between 1 and x (for x>0), else returns an empty matrix   
// Inputs : 
//  x : a real scalar 
// output :
// y : a vector of reals (positives)
//
// F.Belahcene

if prod(size(x,"*"))<>1 | type(x)<>1 
	error("input agument must be a real scalar")
elseif x<1 
	y = [];
else
	y = 1:fix(x);
	ylength = size(y,'*')
	i = 2
	while (i <= sqrt(x))
		if y(i)<> 0
			y(2*y(i):y(i):ylength) = 0
  		end
  		i = i+1
	end
 	y(y<=1)=[];
end

endfunction
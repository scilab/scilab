function y=factor(x)
// Copyright INRIA
// FACTOR function
// Given a real x , this function returns a vector whose the components are the primes factors of the x decompositon.
// Particular cases: 
// if x=0, then returns 0
// if x=1, then return 1
// Inputs : 
//  x : a real scalar (positive) 
// output :
// y : a vector of primes numbers 
//
// F.Belahcene

if prod(size(x,"*"))<>1 | type(x)<>1  
	error("input argument must be a positive integer")
elseif x<0 | x-floor(x)<>0
	error("input argument must be a positive integer")
elseif or(x==[0 1 2 3])
	y=x
else
	xprimefact = primes(sqrt(x))
	y = []
	while x > 1
		xdiv = x ./ xprimefact
		xprimefact_ind = find(xdiv-floor(xdiv)==0)
		if xprimefact_ind <> []
			xprimefact = xprimefact(xprimefact_ind)
			y = [xprimefact y]
			x = x/prod(xprimefact)
		else
			y=[x y]
			break
		end
	end
end
y = gsort(y,'c','i')

endfunction
function y=factorial(n)
// Copyright INRIA
// This function returns the factorial n. If n is a vector, matrix or hypermatrix it returns the element wise factorial
// Input : n, a scalar/vector/matrix/hypermat of positives integers.
// Output : y, a scalar/vector/matrix/hypermat
// F.B

if (type(n) <> 1 & (typeof(n)<>"hypermat" | type(n.entries)<>1)) | (~isempty(n) & (or((n-floor(n)<>0)) | or(n<0))) then 
	error(msprintf(gettext("%s: Wrong value for input argument: Scalar/vector/matrix/hypermatrix of positive integers expected.\n"),"factorial"));
elseif isempty(n)
	y=n
	return
else
	n(n==0)=1
	ntemp=cumprod(1:max(n))
	y=matrix(ntemp(n(:)),size(n))
end

endfunction

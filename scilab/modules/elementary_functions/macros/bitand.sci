function z=bitand(x,y)
// Copyright INRIA
// BITAND function
// Given x,y two positives integers this function returns the decimal number whose the binary form is the AND of the binary representations of x and y

// -Inputs : 
//  x, y :  scalars/vectors/matices/hypermatices of positives integers, x and y must have the same size
// -Output :
//  z : scalar/vector/matrix/hypermatice
//
// F.Belahcene

if size(x)<>size(y)
	error("inputs arguments must have the same size")
elseif isempty(x) & isempty(x)
	z=[]
	return
end

if (type(x)==1 & (x-floor(x)<>0 | x<0)) | (type(x)==8 & x<0) | (type(x)==17 & (type(x.entries<>1) | type(x.entries<>8)) & find(x.entries>0)<>[])  | (type(x)<>1 & type(x)<>8 & type(x)<>17)
	error("first input argument must be a scalar/matrix/hypermatrix of positives integers")
end

if (type(y)==1 & (y-floor(y)<>0 | y<0)) | (type(y)==8 & y<0) | (type(y)==17 & (type(y.entries<>1) | type(y.entries<>8)) & find(y.entries>0)<>[]) | (type(y)<>1 & type(y)<>8 & type(y)<>17)
	error("second input argument must be a scalar/matrix/hypermatrix of positives integers")
end

for i=1:prod(size(x))
	zbin=dec2bin([x(i);y(i)])
	zand=strcat((string(sum(asciimat(zbin)-48,1))))
	zand=strsubst(zand,'1','0')
	zand=strsubst(zand,'2','1')
	z(i)=bin2dec(zand)
end
z=matrix(z,size(x))
endfunction
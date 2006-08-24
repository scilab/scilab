function z=bitxor(x,y)
// Copyright INRIA
// BITXOR function
// Given x,y two positives integers this function returns the decimal number whose the binary form is the XOR of the binary representations of x and y

// -Inputs : 
//  x, y :  scalars/vectors/matices/hypermatices of positives integers, x and y must have the same size
// -Output :
//  z : scalar/vector/matrix/hypermatice
//
// F.Belahcene

if typeof(x)<>typeof(y)
	error("inputs arguments must have the same type")
end
if typeof(x)<>"uint8" & typeof(x)<>"uint16" & typeof(x)<>"uint32" & typeof(x)<>"constant"
	error("type of inputs arguments must be unsigned integers or positives reals")
end

if size(x)<>size(y)
	error("inputs arguments must have the same size")
elseif isempty(x) & isempty(x)
	z=[]
	return
end

if (type(x)==1 & (x-floor(x)<>0 | x<0)) | (type(x)==8 & x<0) | (type(x)==17 & (type(x.entries<>1) | type(x.entries<>8)) & find(x.entries>0)<>[])  | (type(x)<>1 & type(x)<>8 & type(x)<>17)
	error("first input argument must be a scalar/vector/matrix of positives integers")
end

if (type(y)==1 & (y-floor(y)<>0 | y<0)) | (type(y)==8 & y<0) | (type(y)==17 & (type(y.entries<>1) | type(y.entries<>8)) & find(y.entries>0)<>[]) | (type(y)<>1 & type(y)<>8 & type(y)<>17)
	error("second input argument must be a scalar/matrix of positives integers")
end

for i=1:prod(size(x))
	zbin=dec2bin([x(i);y(i)])
	zand=strcat((string(sum(asciimat(zbin)-48,1))))
	zand=strsubst(zand,'2','0')
	z(i)=bin2dec(zand)
end
z=matrix(z,size(x))

if typeof(x)=="uint8"
	z=uint8(z)
elseif typeof(x)=="uint16"
	z=uint16(z)
elseif typeof(x)=="uint32"
	z=uint32(z)
end

endfunction
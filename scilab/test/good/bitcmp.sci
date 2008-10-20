function y=bitcmp(x,n)
// Copyright INRIA
// BITCMP function
// Given an unsigned integer x, this function returns the unsigned integer which is the integer corresponding to the complementary of the binary form of x
// If the bits number of the x binary representation is less than the bitmax number (8,16 or 32) then the bits '1' are added to the complementary in order to have bitmax number (8, 16 or 32) for the complementary 
// for example for the type uint8 (bitmax=8), the complementary of '1101' is not '0010' but '11110010'
// The integer n sets the bits max number
// -Inputs : 
//  x : an unsigned integer
//  n : a positive integer between 1 and the bitmax of the x type
//
// -Output :
//  y : an unsigned integer 
//
// F.Belahcene

rhs=argn(2)
// case empty matrix
if rhs==1 & isempty(x) 
	y=[]
	return
elseif rhs==2 & isempty(x) 
	if type(n)<>1 & type(n)<>8
		error("second input argument must be a positive integer")
	else
		y=[]
		return
	end
end
// check type of first input
if typeof(x)=="uint8"
	sizemax=8
	one=uint8(1)
elseif typeof(x)=="uint16"
	sizemax=16
	one=uint16(1)
elseif typeof(x)=="uint32"
	sizemax=32
	one=uint32(1)
else
	error("first input argument must be a unsigned integer")
end

// check type of second input
if rhs==1
	n=sizemax
elseif rhs==2
	if (type(n)<>1 & type(n)<>8) | n>sizemax | n<1 | n-floor(n)<>0 | prod(size(n))<>1
		error("second input argument must be a positive integer between 1 and "+ string(sizemax))
	end
end

xbin=dec2bin(x,sizemax)
xbin=part(xbin,sizemax-n+1:length(xbin(1)))
xbin=strsubst(xbin,'1','2')
xbin=strsubst(xbin,'0','1')
xbin=strsubst(xbin,'2','0')
y=bin2dec(xbin)*one
y=matrix(y,size(x))
endfunction
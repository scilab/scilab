function y=bitget(x,pos)
// Copyright INRIA
// BITGET function
// Given an unsigned integer x, this function returns an unsigned integer (0 or 1) which is the bit number 'pos' from the representation binary of x
// if x=uint8(19) and pos=2 then bitget returns the 2th bit of the binary form of 19 ('10011') which is 1  
// -Inputs : 
//  x : an unsigned integer
// pos : a positive integer between 1 and the bitmax of the x type
//
// -Output :
//  y : an unsigned integer 
//
// F.Belahcene

rhs=argn(2)
// check rhs number
if rhs<>2
	error(msprintf(gettext("%s: Wrong number of input arguments. %d expected.\n"),"bitget",2));
end
// case empty matrix
if isempty(x)
	if ~isempty(pos) & prod(size(pos))<>1
		error(msprintf(gettext("%s: Wrong size for inputs arguments: Same sizes expected.\n"),"bitget"));
	else
		y=[]
		return
	end
end

// check the type of the first input argument
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
	error(msprintf(gettext("%s: Wrong type for first input argument: Unsigned integer expected.\n"),"bitget"));
end

// check the type and size of the second input argument
if prod(size(pos))==1 
	pos=ones(x)*pos
elseif size(pos)<>size(x)
	error(msprintf(gettext("%s: Wrong size for inputs arguments: Same sizes expected.\n"),"bitget"));
end
if (type(pos)<>1 & type(pos)<>8) | or(pos>sizemax) | or(pos<1) | or(pos-floor(pos)<>0) 
	error(msprintf(gettext("%s: Wrong value for second input argument: Positive integers between 1 and %d expected.\n"),"bitget",sizemax));
end

xbin=dec2bin(x,sizemax)
for i=1:prod(size(xbin))
	ytemp=asciimat(xbin(i))-48
	y(i)=ytemp(:,sizemax+1-pos(i))
end
y=matrix(y,size(x))
y=one*y

endfunction

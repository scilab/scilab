function y=bin2dec(str)
// Copyright INRIA
// BIN2dec function
// Given str a binary string, this function returns the decimal number whose the binary representation is given by str

// -Input : 
//  str : a string (or a vector/matrix of strings), containing only characters '1' and '0' 
// -Output :
//  y : a scalar/vector/matrix
//
// F.Belahcene

// check the type of input argument
if type(str)<>10 
	error("input argument must be a string of characters ''0'' or ''1'' ")
end

// delete all spaces included in the str
str=strsubst(str,' ','')

// check the str characters are only '0' or '1', and convert the binary str to corresponing decimal number  
for i=1:prod(size(str))
	ind1=strindex(str(i),'1')
	ind0=strindex(str(i),'0')
	if length(str(i))<>sum([prod(size(ind0)) prod(size(ind1))])
		error("input argument must be a string of characters ''0'' or ''1'' ")
	end
	
	if ~isempty(ind1)
		ind1=length(str(i))-ind1($:-1:1)
		y($+1)=sum(2^ind1)
	elseif ~isempty(ind0)
		y($+1)=0
	else
		y($+1)=[]
	end
end
y=matrix(y,size(str))

endfunction
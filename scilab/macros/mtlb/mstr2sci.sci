function y=mstr2sci(x)
// Copyright INRIA
// Conversion of a Matlab format string into a Scilab string
// In Matlab : 'string' has size [1 6]
// In Scilab : 'string' has size [1 1]
// This function solves this problem by converting a string into a character matrix
// Ex : 'string' is converted into ['s' 't' 'r' 'i' 'n' 'g']

if type(x)==17 then disp("Not written for hypermatrices !"),return,end
if type(x)<>10 then disp("Input argument must be a caracter string matrix !"),return,end
nr=size(x,1);
nc=max(length(x));
y=[]
for k=1:nr
  x(k,:)=strcat(x(k,:));
  nc=max(length(x(k,:)));
  tmp = ascii(x(k,:));
  for l=1:nc
    y(k,l)=ascii(tmp(l));
  end
end

endfunction

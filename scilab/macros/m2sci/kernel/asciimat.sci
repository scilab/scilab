function y=asciimat(x)
// Copyright INRIA
// This function converts a matrix of string into a matrix of ascii codes using ascii() Scilab function
// Returned value have same size as input value instead of last dim !
// Fonction created because ascii() Scilab function returns a row vector

xsize=size(x);

if size(x,1)<>1 then
  xsize=[xsize(1:$-1),-1]
  xsize=xsize($:-1:1)
  y=matrix(ascii(x),xsize)';
else
  y=matrix(ascii(x),[xsize(1:$-1),-1]);
end
endfunction

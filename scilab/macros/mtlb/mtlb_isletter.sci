function tf=mtlb_isletter(A)
// Copyright INRIA
// Translation function for isletter() Matlab function
// V.C.

if type(A)==10 then
  tf = (asciimat(A)>=65&asciimat(A)<=90)|(asciimat(A)>=97&asciimat(A)<=122)
else
  tf = zeros(A)
end
endfunction

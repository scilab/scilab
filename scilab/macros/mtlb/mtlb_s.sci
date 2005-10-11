function y=mtlb_s(a,b)
// Copyright INRIA
// Emulation function for Matlab substraction
// V.C.

  if a==[] | b==[] then
    y=[];
  else
    y=a-b;
  end
endfunction


function y=mtlb_l(a,b)
// Copyright INRIA
// Emulation function for Matlab left division
// V.C.

// Warning: this function should not work if only a or b is a string matrix...
// In these case Matlab result is very strange !

  transpose=%F

  if type(a)==10 & type(b)==10 then 
    transpose=%T
  end
  if type(a)==10 then
    a=asciimat(a)'
  end
  if type(b)==10 then
    b=asciimat(b)'
  end

  if or(type(a)==[4,6]) then
    a=bool2s(a)
  end
  if or(type(b)==[4,6]) then
    b=bool2s(b)
  end

  y=a\b

  if transpose then
    y=y($:-1:1,$:-1:1)'
  end
endfunction

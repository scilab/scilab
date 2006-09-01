function a=%r_clean(a,epsa,epsr)
//Syntax: a=%r_ clean(a,epsa,epsr)
// Given a, matrix of rationals , this function 
// eliminates  all the coefficients of a with absolute value < epsa 
// and realtive value < epsr  (relative means realive wrt  norm 1 of
// the coefficients)
// Default values : epsa=1.d-10; epsr=1.d-10;
//!

// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs == 1 then
  epsa=1.d-10;
  epsr=1.d-10;
elseif rhs==2 then
  epsr=1.d-10;
end
tdom=a('dt')
a=simp(clean(a('num'),epsa,epsr)./clean(a('den'),epsa,epsr));a('dt')=tdom 
endfunction

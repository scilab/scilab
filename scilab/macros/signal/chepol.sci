function [Tn]=chepol(n,var)
//Recursive implementation of Chebychev polynomial
//  n   :Polynomial order
//  var :Polynomial variable (character string)
//  Tn  :Polynomial in var
//
//!
//Author F.D.
// Copyright INRIA
T1=poly(0,var);
T0=1+0*T1;
if n==0 then,
      Tn=T0,
      return,
end,
if n==1 then,
      Tn=T1,
      return,
end,
if n>1 then,
      Tn=2*poly(0,var)*chepol(n-1,var)-chepol(n-2,var),
end
    



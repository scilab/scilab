function [t]=soundsec (n,rate)
// Copyright INRIA
// Return n seconds of t parameter at rate rate (sample/sec)
  [lhs,rhs]=argn(0);
  if rhs <=1 ; rate=22050; end;
  t= linspace(0,n,n*rate);
endfunction

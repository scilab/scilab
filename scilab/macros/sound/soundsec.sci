function [t]=soundsec (n,rate)
// Copyright INRIA
// Return n seconds of t parameter.
[lhs,rhs]=argn(0);
defaultrate=22050;
if rhs <=1 ; rate=defaultrate; end;
t= 0:2*%pi/rate:2*n*%pi;

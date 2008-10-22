% Test file for function feval()
% Copyright INRIA
% Matlab version: 6.5.0.180913a (R13)
% V.C.

[y1] = feval(@cos,0)
[y1] = feval(@size,1,1)
[y1,y2] = feval(@size,1)

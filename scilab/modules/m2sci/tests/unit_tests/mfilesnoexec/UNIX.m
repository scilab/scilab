% Test file for function unix()
% Copyright INRIA
% Matlab version: 6.5.0.180913a (R13)
% V.C.

unix('ls')
status = unix('ls')
status = unix('ls','-echo')

[status,result] = unix('ls')
[status,result] = unix('ls','-echo')

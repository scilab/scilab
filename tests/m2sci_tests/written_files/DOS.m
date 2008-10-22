% Test file for function dos()
% Copyright INRIA
% Matlab version: 6.5.0.180913a (R13)
% V.C.

dos('ls')
status = dos('ls')
status = dos('ls','-echo')

[status,result] = dos('ls')
[status,result] = dos('ls','-echo')

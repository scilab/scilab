% Test file for function ferror()
% Copyright INRIA
% Matlab version: 6.5.0.180913a (R13)
% V.C.

fid=1
message = ferror(fid)
message = ferror(fid,'clear')
[message,errnum] = ferror(fid)
[message,errnum] = ferror(fid,'clear')


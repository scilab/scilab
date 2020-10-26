% Test file for function ferror()
% Copyright INRIA
% Matlab version: 6.5.0.180913a (R13)
% V.C.

fid=1
Message = ferror(fid)
Message = ferror(fid,'clear')
[Message, errnum] = ferror(fid)
[Message, errnum] = ferror(fid,'clear')


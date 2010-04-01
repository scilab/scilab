% Test file for function fwrite()
% Copyright INRIA
% Matlab version: 6.5.0.180913a (R13)
% V.C.

fid=1
fwrite(fid,ones(5),'integer*4');
count=fwrite(fid,ones(5),'integer*4');

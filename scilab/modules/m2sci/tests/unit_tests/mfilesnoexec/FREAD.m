% Test file for function fread()
% Copyright INRIA
% Matlab version: 6.5.0.180913a (R13)
% V.C.

fid=1
s = fread(fid,120,'uchar');
[s,count] = fread(fid,120,'uchar');
s = fread(fid,120,'40*uchar=>uchar',8);
[s,count] = fread(fid,120,'40*uchar=>uchar',8);

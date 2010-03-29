% Test file for function fseek()
% Copyright INRIA
% Matlab version: 6.5.0.180913a (R13)
% V.C.

fid=1
status = fseek(fid,0,'bof');
status = fseek(fid,0,'cof');
status = fseek(fid,0,'eof');
status = fseek(fid,0,-1);
status = fseek(fid,0,0);
status = fseek(fid,0,1);
origin=1;
status = fseek(fid,0,origin);

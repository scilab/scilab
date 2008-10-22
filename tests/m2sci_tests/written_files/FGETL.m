% Test file for function fgetl()
% Copyright INRIA
% Matlab version: 7.0.0.19901 (R14)
% V.C.

fid=1;

% Example 1
fgetl(fid);

% Example 2
if fgetl(fid)==-1
  disp('eof');
end

% Example 3
l=fgetl(fid);

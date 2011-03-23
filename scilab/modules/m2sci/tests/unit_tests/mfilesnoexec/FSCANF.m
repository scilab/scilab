% Test file for function fscanf()
% Copyright INRIA
% Matlab version: 6.5.0.180913a (R13)
% V.C.

fid=1
a = fscanf(fid,'%g %g');
a = fscanf(fid,'%g %g',[2 inf]);
[a,count] = fscanf(fid,'%g %g');
[a,count] = fscanf(fid,'%g %g',[2 inf]);


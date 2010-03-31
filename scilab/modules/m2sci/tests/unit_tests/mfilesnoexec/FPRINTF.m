% Test file for function fprintf()
% Copyright INRIA
% Matlab version: 6.5.0.180913a (R13)
% V.C.

fid=1
x = 0:.1:1;
y = [x; exp(x)];
count=fprintf(fid,'%6.2f %12.8f\n',y);
count=fprintf('%6.2f %12.8f\n',y);

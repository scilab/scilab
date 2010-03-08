% Test file for function fclose()
% Copyright INRIA
% Matlab version: 6.5.0.180913a (R13)
% V.C.

fid = 10;
fclose('all')
fclose(1)
fclose(fid)
status = fclose('all')
status = fclose(1)
status = fclose(fid)

% Test file for function fopen()
% Copyright INRIA
% Matlab version: 6.5.0.180913a (R13)
% V.C.

filename='todelete1.tst';

fid=fopen(filename,'w');

fid=fopen(filename,'r');

fid=fopen(filename,'a');

fid=fopen(filename,'r+');

fid=fopen(filename,'w+');

fid=fopen(filename,'a+');

fid=fopen(filename,'A');

fid=fopen(filename,'W');

fid=fopen(filename);

[fid,mess]=fopen(filename);

permission='r';
machineformat='ieee-le';

[fid,mess]=fopen(filename,permission);

fid=fopen(filename,permission,machineformat);

[fid,mess]=fopen(filename,permission,machineformat);

fids=fopen('all');

[file_name,permission,machineformat]=fopen(fid);


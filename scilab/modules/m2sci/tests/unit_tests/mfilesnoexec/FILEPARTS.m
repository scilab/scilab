% Test file for function fileparts()
% Copyright INRIA
% Matlab version: 7.0.0.19901 (R14)
% V.C.

file = '\home\user4\matlab\classpath.txt';

fileparts(file)

[pathstr] = fileparts(file)

[pathstr,name] = fileparts(file)

[pathstr,name,ext] = fileparts(file)

[pathstr,name,ext,versn] = fileparts(file)
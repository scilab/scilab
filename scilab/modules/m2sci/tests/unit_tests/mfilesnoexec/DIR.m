% Test file for function dir()
% Copyright INRIA
% Matlab version: 6.5.0.180913a (R13)
% V.C.

dirname='~';
dir
dir dirname
dir(dirname)
dir 'dirname'
dir('dirname')
files = dir
files1 = dir(dirname)
files2 = dir('dirname')




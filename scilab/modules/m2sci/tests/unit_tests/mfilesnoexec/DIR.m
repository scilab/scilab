% Test file for function dir()
% Copyright INRIA
% Matlab version: 6.5.0.180913a (R13)
% V.C.

dirName = '~';
dir
dir dirname
dir(dirName)
dir 'dirname'
dir('dirname')
files = dir
files1 = dir(dirName)
files2 = dir('dirname')




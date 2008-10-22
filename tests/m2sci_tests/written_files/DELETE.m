% Test file for function delete()
% Copyright INRIA
% Matlab version: 6.5.1.199709 (R13) Service Pack 1
% V.C.

% File deleting
filename='directory/filename.ext'
delete(filename)

delete directory/filename.ext

% Graphics objects deleting
%m2scideclare h|? ?|Handle
delete(h)

% Other inputs
%m2scideclare h1|? ?|?
delete(h1)
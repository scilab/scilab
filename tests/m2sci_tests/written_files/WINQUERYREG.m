% Test file for function winqueryreg()
% Copyright INRIA
% Matlab version: 7.0.0.19901 (R14)
% V.C.

winqueryreg('name', 'rootkey', 'subkey')
winqueryreg('rootkey', 'subkey', 'valname')
winqueryreg('rootkey', 'subkey')
value = winqueryreg('name', 'rootkey', 'subkey')
value = winqueryreg('rootkey', 'subkey', 'valname')
value = winqueryreg('rootkey', 'subkey')

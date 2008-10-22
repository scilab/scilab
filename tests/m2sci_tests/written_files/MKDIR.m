% Test file for function mkdir()
% Copyright INRIA
% Matlab version: 7.0.0.19901 (R14)
% V.C.

mkdir('dirname')
mkdir('parentdir','dirname')
[status] = mkdir('dirname')
[status] = mkdir('parentdir','dirname')
[status,message] = mkdir('dirname')
[status,message] = mkdir('parentdir','dirname')
[status,message,messageid] = mkdir('dirname')
[status,message,messageid] = mkdir('parentdir','dirname')

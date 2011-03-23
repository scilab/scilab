% Test file for function isequal()
% Copyright INRIA
% Matlab version: 7.0.0.19901 (R14)
% V.C.

%m2scideclare A|Unknown Unknown|Unknown|Unknown
%m2scideclare B|Unknown Unknown|Unknown|Unknown
%m2scideclare C|Unknown Unknown|Unknown|Unknown
isequal(A,B)
tf = isequal(A,B)
isequal(A,B,C)
tf = isequal(A,B,C)
%m2scideclare D|Unknown Unknown|Struct|Unknown
%m2scideclare E|Unknown Unknown|Struct|Unknown
%m2scideclare F|Unknown Unknown|Struct|Unknown
isequal(D,E)
tf = isequal(D,E)
isequal(D,E,F)
tf = isequal(D,E,F)

% Test file for function num2str
% Copyright INRIA
% Matlab version: 6.5.1.199709 (R13) Service Pack 1
% V.C.

%m2scideclare a|Unknown Unknown|Double
%m2scideclare myprecision|Unknown Unknown|Double
%m2scideclare myformat|1 Unknown|String

str1 = num2str(a)
str2 = num2str(a,3)
str3 = num2str(a,myprecision)
str4 = num2str(a,myformat)
str5 = num2str(a,'%3d')
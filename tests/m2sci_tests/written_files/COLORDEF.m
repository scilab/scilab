% Test file for function colordef
% Copyright INRIA
% Matlab version: 6.5.1.199709 (R13) Service Pack 1
% V.C.

colordef white
colordef black
colordef none
f = colordef('white')
f = colordef('black')
f = colordef('none')
%m2scideclare fig|1 1|Handle
colordef(fig,'white')
colordef(fig,'black')
colordef(fig,'none')
f = colordef(fig,'white')
f = colordef(fig,'black')
f = colordef(fig,'none')
colordef('new','white')
colordef('new','black')
colordef('new','none')
h = colordef('new','white')
h = colordef('new','black')
h = colordef('new','none')

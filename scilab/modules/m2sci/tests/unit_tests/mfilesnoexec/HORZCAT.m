% Test file for function horzcat
% Copyright INRIA
% Matlab version: 7.0.0.19901 (R14)
% V.C.

A=horzcat(1,1)
B=horzcat(1,i)
%m2scideclare x1|1 10|Double|Unknown
C=horzcat(1,x1)
%m2scideclare y1|10 10|Double|Real
%m2scideclare y2|Unknown 10|Double|Real
D=horzcat(y1,y2)
%m2scideclare a1|Unknown 10|Double|Real
%m2scideclare a2|Unknown 10|Double|Real
E=horzcat(a1,a2)
%m2scideclare b1|Unknown Unknown|Double|Real
%m2scideclare b2|Unknown Unknown|Double|Real
F=horzcat(b1,b2)
%m2scideclare c1|10 5|String
%m2scideclare c2|10 10|String
%m2scideclare c3|10 2|String
G=horzcat(c1,c2,c3)
%m2scideclare d1|10 5 20|Double|Real
%m2scideclare d2|10 10 20|Double|Real
%m2scideclare d3|10 2 20|Double|Real
H=horzcat(d1,d2,d3)

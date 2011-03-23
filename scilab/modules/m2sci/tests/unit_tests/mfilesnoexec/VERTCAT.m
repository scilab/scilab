% Test file for function vertcat
% Copyright INRIA
% Matlab version: 7.0.0.19901 (R14)
% V.C.

A=vertcat(1,1)
B=vertcat(1,i)
%m2scideclare x1|10 1|Double|Unknown
C=vertcat(1,x1)
%m2scideclare y1|10 10|Double|Real
%m2scideclare y2|10 Unknown|Double|Real
D=vertcat(y1,y2)
%m2scideclare a1|10 Unknown|Double|Real
%m2scideclare a2|10 Unknown|Double|Real
E=vertcat(a1,a2)
%m2scideclare b1|Unknown Unknown|Double|Real
%m2scideclare b2|Unknown Unknown|Double|Real
F=vertcat(b1,b2)
%m2scideclare c1|5 10|String
%m2scideclare c2|10 10|String
%m2scideclare c3|2 10|String
G=vertcat(c1,c2,c3)
%m2scideclare d1|5 10 20|Double|Real
%m2scideclare d2|10 10 20|Double|Real
%m2scideclare d3|2 10 20|Double|Real
H=vertcat(d1,d2,d3)

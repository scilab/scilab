% Test file for function isfield()
% Copyright INRIA
% Matlab version: 7.0.0.19901 (R14)
% V.C.

%m2scideclare ST1|Unknown Unknown|Unknown|Unknown
%m2scideclare F1|Unknown Unknown|Unknown|Unknown
isfield(ST1,F1)
tf1 = isfield(ST1,F1)
%m2scideclare ST2|Unknown Unknown|Struct|Unknown
%m2scideclare F2|Unknown Unknown|Unknown|Unknown
isfield(ST2,F2)
tf2 = isfield(ST2,F2)
%m2scideclare ST3|Unknown Unknown|Unknown|Unknown
%m2scideclare F3|Unknown Unknown|String|Unknown
isfield(ST3,F3)
tf3 = isfield(ST3,F3)
%m2scideclare ST4|Unknown Unknown|Struct|Unknown
%m2scideclare F4|Unknown Unknown|String|Unknown
isfield(ST4,F4)
tf4 = isfield(ST4,F4)
%m2scideclare ST5|Unknown Unknown|Double|Unknown
%m2scideclare F5|Unknown Unknown|Unknown|Unknown
isfield(ST5,F5)
tf5 = isfield(ST5,F5)
%m2scideclare ST6|Unknown Unknown|Unknown|Unknown
%m2scideclare F6|Unknown Unknown|Double|Unknown
isfield(ST6,F6)
tf6 = isfield(ST6,F6)

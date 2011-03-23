% Test file for function isa()
% Copyright INRIA
% Matlab version: 6.5.1.199709 (R13) Service Pack 1
% V.C.

%m2scideclare x|? ?|?|?
a=isa(x,'logical')
b=isa(x,'char')
c=isa(x,'numeric')
d=isa(x,'int8')
e=isa(x,'uint8')
f=isa(x,'int16')
g=isa(x,'uint16')
h=isa(x,'int32')
k=isa(x,'uint32')
l=isa(x,'single')
m=isa(x,'double')
n=isa(x,'cell')
o=isa(x,'struct')
p=isa(x,'function_handle')
q=isa(x,'sparse')
r=isa(x,'lti')
s=isa(x,'user_defined_class')
%m2scideclare class|? ?|?|?
t=isa(x,class)

function y=mtlb_strrep(s1,s2,s3)
// Copyright INRIA
// Emulation function for strrep() Matlab function
// V.C.

if type(s1)==10 & type(s2)==10 & type(s3)==10 then
  y=strsubst(s1,s2,s3)
elseif type(s1)==10 | type(s2)==10 | type(s3)==10 then
  y=s1
else
  y=matrix(s1,1,-1)
  y(y==s2)=s3
end
endfunction

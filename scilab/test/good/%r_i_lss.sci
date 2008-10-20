function s2=%r_i_lss(i,j,s1,s2)
//%r_i_lss(i,j,s1,s2) <=> s2(i,j)=s1
//!
// origine s. steer inria 1992
//
// Copyright INRIA
if type(i)==10|type(j)==10 then 
  error(21)
end
[s1 s2]=sysconv(s1,s2)
s2(i,j)=s1
endfunction

function [s2]=%lss_i_r(i,j,s1,s2)
//s2=%lss_i_r(i,j,s1,s2) <=>  s2(i,j)=s1
//!
// origin s. steer inria 1992
// Copyright INRIA
[s1 s2]=sysconv(s1,s2)
s2(i,j)=s1
endfunction

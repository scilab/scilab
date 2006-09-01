function [s]=%lss_s_r(s1,s2)
//s=%lss_s_r(s1,s2) <=> s=s1-s2
// s1 : state-space
// s2 : transfer matrix
//!
// Copyright INRIA
[s1,s2]=sysconv(s1,s2)
s=s1-s2
endfunction
